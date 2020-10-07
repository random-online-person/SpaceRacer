#include "glew.h"
#include "glfw3.h"
#include "SOIL2.h"

#include "Shader.h"
#include "Camera.h"
#include "Input.h"
#include"Model.h"
#include "Skybox.h"
#include "ObjectMovement.h"

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::vector<std::string> Skyboxs{
	"Images/SkyBox/right.jpg",
	"Images/SkyBox/left.jpg",
	"Images/SkyBox/top.jpg",
	"Images/SkyBox/bottom.jpg",
	"Images/SkyBox/front.jpg",
	"Images/SkyBox/back.jpg"
};		 

std::vector<std::string> SkyboxsSpaceMaxPro{
	"Images/bkg/blue/bkg1_right.png",
	"Images/bkg/blue/bkg1_left.png",
	"Images/bkg/blue/bkg1_top.png",
	"Images/bkg/blue/bkg1_bot.png",
	"Images/bkg/blue/bkg1_front.png",
	"Images/bkg/blue/bkg1_back.png"
};


float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	float renderDistance = 10000.0f;


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (glewInit())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SkyBox skybox(SkyboxsSpaceMaxPro);

	Model ourModel("OBJFile/starship.obj");
	Model test("OBJFile/Rock2/Rock1.obj");
	Model Rock("OBJFile/Asteroid/10464_Asteroid_v1_Iterations-2.obj");
	//Model Rock2("OBJFile/Rock1/test.obj");

	//5166 51660 ship
	//245152 3677282 bag
									   
	const int amount = 1000;
	const int Rocknum = 50;

	struct objectprop
	{
		glm::vec3 position;
		glm::vec3 moveDir;
		glm::vec3 scale;
		float rotspeed;
		float movespeed;
	};

	glm::mat4 modelmat[amount];
	objectprop objects[amount];

	glm::mat4 Rockmat[Rocknum];
	objectprop RockObj[Rocknum];

	int Xhigh = 10000, Xlow = 0;
	int Yhigh = 5000, Ylow = -5000;
	int Zhigh = 5000, Zlow = -5000;
	int Shigh = 1000, Slow = 200;

	for (int i = 0; i < Rocknum; i++) {
		RockObj[i].position.x = (rand() % (Xhigh - Xlow + 1) + Xlow);
		RockObj[i].position.y = (rand() % (Yhigh - Ylow + 1) + Ylow);
		RockObj[i].position.z = (rand() % (Zhigh - Zlow + 1) + Zlow);

		glm::vec3 temp;

		temp.x = (rand() % (Xhigh - Xlow + 1) + Xlow);
		temp.y = (rand() % (Yhigh - Ylow + 1) + Ylow);
		temp.z = (rand() % (Zhigh - Zlow + 1) + Zlow);

		RockObj[i].moveDir = glm::normalize(temp);

		int tempint = (rand() % (Shigh - Slow + 1) + Slow);

		RockObj[i].scale = glm::vec3((float)tempint / 1000.0f);
		//RockObj[i].scale = glm::vec3(0.50f);
		RockObj[i].rotspeed = rand();
		RockObj[i].movespeed = (1.0f / (float)tempint) * 1000;
	}

	for (int i = 0; i < amount; i++) {

		objects[i].position.x = (rand() % (Xhigh - Xlow + 1) + Xlow);
		objects[i].position.y = (rand() % (Yhigh - Ylow + 1) + Ylow);
		objects[i].position.z = (rand() % (Zhigh - Zlow + 1) + Zlow);	

		glm::vec3 temp;

		temp.x = (rand() % (Xhigh - Xlow + 1) + Xlow);
		temp.y = (rand() % (Yhigh - Ylow + 1) + Ylow);
		temp.z = (rand() % (Zhigh - Zlow + 1) + Zlow);

		objects[i].moveDir = glm::normalize(temp);

		int tempint = (rand() % (Shigh - Slow + 1) + Slow);

		//objects[i].scale = glm::vec3((float)tempint / 1000.0f);
		objects[i].scale = glm::vec3(50.0f);
		objects[i].rotspeed = rand();
		objects[i].movespeed = (1.0f / (float)tempint) * 1000;
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	// set transformation matrices as an instance vertex attribute (with divisor 1)
	// note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
	// normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
	// -----------------------------------------------------------------------------------------------------------------------------------
	for (unsigned int i = 0; i < test.GetMeshSize(); i++)
	{
		unsigned int VAO = test.GetVAO(i);
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));	  
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(sizeof(glm::mat4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}


	unsigned int Rockbuffer;
	glGenBuffers(1, &Rockbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, Rockbuffer);
	glBufferData(GL_ARRAY_BUFFER, Rocknum * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	// set transformation matrices as an instance vertex attribute (with divisor 1)
	// note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
	// normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
	// -----------------------------------------------------------------------------------------------------------------------------------
	for (unsigned int i = 0; i < Rock.GetMeshSize(); i++)
	{
		unsigned int VAO = Rock.GetVAO(i);
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(sizeof(glm::mat4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}




	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
	Shader highlightshader("Shaders/VertexShader.glsl", "Shaders/HighLightFragment.glsl");
	Shader SkyboxShader("Shaders/SkyBoxVertex.glsl", "Shaders/SkyBoxFragment.glsl");
	Shader TransparentShader("Shaders/TransparentVertex.glsl", "Shaders/TransParentFragment.glsl");
	Shader AfterEffects("Shaders/AfterEffectsVertex.glsl", "Shaders/AfterEffectsFragment.glsl");
	Shader AsteroidShader("Shaders/InstanceVertex.glsl", "Shaders/InstanceFragment.glsl");
	Shader TempShader("Shaders/TempVertex.glsl", "Shaders/TempFragment.glsl");
	//Shader Light("Shaders/LightVertex.glsl", "Shaders/LightFragment.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	Input input;

	float softangle = 35.0f;
	float hardangle = 45.5f;

	//SkyBox cubeBox(Skyboxs, SkyboxShader);

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), renderDistance);

	glm::vec3 lightpos(20.f, -20.f, -10.f);
	glm::vec3 lightcolor(1.0f);


	glm::vec3 ambient(0.1f, 0.5f, 0.8f);
	ambient = ambient * 0.1f;
	glm::vec3 diffuse(0.5f);
	glm::vec3 specular(0.5f);
	float shininess = 32.0f;

	skybox.AttachShader(SkyboxShader);

	TransparentShader.Use();
	TransparentShader.setInt("skybox", 0);

	glm::vec3 scale(0.3f, 0.3f, 0.3f);
	glm::vec3 scalediff(0.01f, 0.01f, 0.01f);

	ObjectMovement ship;




	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		input.Update(window);

		// render
		// ------





		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//glEnable(GL_DEPTH_TEST);








		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//camera.Update(window ,input.GetDeltaTime(), input.GetMouseOffsetX(), input.GetMouseOffsetY());
		
		ship.UpdateObject(window, input.GetDeltaTime(), input.GetMouseOffsetX(), input.GetMouseOffsetY());
		camera.Follow(ship.GetPosition(), ship.GetYaw(), ship.GetPitch(), 100.0f, 0.0f, -30.0f);
		
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(5000.f, 0.f, 0.f));
		//model = glm::rotate(model, glm::radians(270.f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		
		skybox.Draw(SkyboxShader, camera);
		// render container


		//tempstruct lol[6];
		//
		//lol[0].position = glm::vec3(0.5f, 0.5f, 0.0f);
		//lol[1].position = glm::vec3(0.5f, -0.5f, 0.0f);
		//lol[2].position = glm::vec3(-0.5f, -0.5f, 0.0f);
		//lol[3].position = glm::vec3(0.5f, 0.5f, 0.0f);
		//lol[4].position = glm::vec3(-0.5f, -0.5f, 0.0f);
		//lol[5].position = glm::vec3(-0.5f, 0.5f, 0.0f);	
		//
		//lol[0].color = glm::vec3(1.0f, 0.0f, 0.0f);
		//lol[1].color = glm::vec3(0.0f, 1.0f, 0.0f);
		//lol[2].color = glm::vec3(0.0f, 0.0f, 1.0f);
		//lol[3].color = glm::vec3(1.0f, 1.0f, 0.0f);
		//lol[4].color = glm::vec3(1.0f, 0.0f, 0.0f);
		//lol[5].color = glm::vec3(0.0f, 1.0f, 0.0f);
		//
		//
		//glm::mat4 paininass(1.0f);
		//
		//paininass = glm::translate(paininass, glm::vec3(00.0f, 50.0f, 0.0f));
		//paininass = glm::rotate(paininass, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//paininass = glm::scale(paininass, glm::vec3(100.f, 100.f, 100.f));
		//
		//TempShader.Use();
		//TempShader.setMat4fv(paininass, "ModelMatrix");
		//TempShader.setMat4fv(camera.GetViewMat(), "ViewMatrix");
		//TempShader.setMat4fv(camera.GetProjectionMat(), "ProjectionMatrix");
		
		//glBindBuffer(GL_ARRAY_BUFFER, unliveVBO);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(tempstruct), lol);
		//
		//glBindVertexArray(unliveVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);


		for (int i = 0; i < amount; i++) {
			objects[i].position += objects[i].moveDir * 5.0f;

			if (objects[i].position.x < camera.getPosition().x) {
				objects[i].position.x = camera.getPosition().x + renderDistance + (rand() % (500 - 100 + 1) + 100);
				objects[i].position.y = (rand() % (Yhigh - Ylow + 1) + Ylow);
				objects[i].position.z = (rand() % (Zhigh - Zlow + 1) + Zlow);
			}

			modelmat[i] = glm::mat4(1.0f);
			modelmat[i] = glm::translate(modelmat[i], objects[i].position);
			modelmat[i] = glm::rotate(modelmat[i], objects[i].rotspeed, objects[i].moveDir);
			modelmat[i] = glm::scale(modelmat[i], objects[i].scale);
		}

		AsteroidShader.Use();

		AsteroidShader.setMat4fv(camera.GetViewMat(), "view");
		AsteroidShader.setMat4fv(camera.GetProjectionMat(), "projection");
		AsteroidShader.setInt("texture_diffuse1", 0);

		AsteroidShader.setVec3("light.position", camera.getPosition());
		AsteroidShader.setVec3("light.direction", camera.getFront());

	

		if (ship.IsBoostOn()) {								  
			softangle = 15.f;
			hardangle = 20.f;  
			diffuse = glm::vec3(0.9f);
			specular = glm::vec3(0.9f);
		}
		else if(!(ship.IsBoostOn()) && softangle <= 35.0f && input.GetDeltaTime() < 0.1f){
			softangle = 30.0f;
			hardangle = 45.0f;
			diffuse = glm::vec3(0.7f);
			specular = glm::vec3(0.7f);
		}

		//std::cout << input.GetDeltaTime() << std::endl;

		AsteroidShader.setF1("light.cutOff", glm::cos(glm::radians(softangle)));
		AsteroidShader.setF1("light.outerCutOff", glm::cos(glm::radians(hardangle)));

		AsteroidShader.setVec3("cameraPos", camera.getPosition());

			

		AsteroidShader.setVec3("light.ambient", ambient);
		AsteroidShader.setVec3("light.diffuse", diffuse);
		AsteroidShader.setVec3("light.specular", specular);
		AsteroidShader.setVec3("light.color", lightcolor);


		AsteroidShader.setF1("light.constant", 1.0f);
		AsteroidShader.setF1("light.linear", 0.09f);
		AsteroidShader.setF1("light.quadratic", 0.032f);


		AsteroidShader.setVec3("material.ambient", ambient);
		AsteroidShader.setVec3("material.diffuse", diffuse);
		AsteroidShader.setVec3("material.specular", specular);
		AsteroidShader.setF1("material.shininess", shininess);

		//AsteroidShader.setVec3("light.position", lightpos);
		//AsteroidShader.setVec3("light.color", lightcolor);


		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(modelmat), modelmat);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, test.GetTextureID(0)); // note: we also made the textures_loaded vector public (instead of private) from the model class.
		for (unsigned int i = 0; i < test.GetMeshSize(); i++)
		{
			glBindVertexArray(test.GetVAO(i));
			glDrawElementsInstanced(GL_TRIANGLES, test.GetMeshIndexSize(i), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}



		for (int i = 0; i < Rocknum; i++) {
			RockObj[i].position += RockObj[i].moveDir * 5.0f;

			if (RockObj[i].position.x < camera.getPosition().x) {
				RockObj[i].position.x = camera.getPosition().x + renderDistance + (rand() % (500 - 100 + 1) + 100);
				RockObj[i].position.y = (rand() % (Yhigh - Ylow + 1) + Ylow);
				RockObj[i].position.z = (rand() % (Zhigh - Zlow + 1) + Zlow);
			}

			Rockmat[i] = glm::mat4(1.0f);
			Rockmat[i] = glm::translate(Rockmat[i], RockObj[i].position);
			Rockmat[i] = glm::rotate(Rockmat[i], RockObj[i].rotspeed, RockObj[i].moveDir);
			Rockmat[i] = glm::scale(Rockmat[i], RockObj[i].scale);
		}

		glBindBuffer(GL_ARRAY_BUFFER, Rockbuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Rockmat), Rockmat);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Rock.GetTextureID(0)); // note: we also made the textures_loaded vector public (instead of private) from the model class.
		for (unsigned int i = 0; i < Rock.GetMeshSize(); i++)
		{
			glBindVertexArray(Rock.GetVAO(i));
			glDrawElementsInstanced(GL_TRIANGLES, Rock.GetMeshIndexSize(i), GL_UNSIGNED_INT, 0, Rocknum);
			glBindVertexArray(0);
		}
		


		//ourShader.Use();
		//
		//ourShader.setVec3("light[0].position", lightpos);
		//ourShader.setVec3("light[0].color", lightcolor);
		//
		//ourShader.setVec3("cameraPos", camera.getPosition());
		//
		////std::cout << camera.GetViewMat().length << std::endl;
		//
		//ourShader.setVec3("material.ambient", ambient);
		//ourShader.setVec3("material.diffuse", diffuse);
		//ourShader.setVec3("material.specular", specular);
		//ourShader.setF1("material.shininess", shininess);
		//
		//
		//ourShader.setMat4fv(camera.GetViewMat(), "view");
		//ourShader.setMat4fv(camera.GetProjectionMat(), "projection");
		//ourShader.setMat4fv(model, "model");
		//
		//test.Draw(ourShader);

														  							   				 
		TransparentShader.Use();
		TransparentShader.setMat4fv(camera.GetViewMat(), "view");
		TransparentShader.setMat4fv(camera.GetProjectionMat(), "projection");
		TransparentShader.setVec3("cameraPos", camera.getPosition());


		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(270.f), glm::vec3(1.0f, 0.0f, 0.0f));
		////model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, scale);	// it's a bit too big for our scene, so scale it down
		//TransparentShader.setMat4fv(model, "model");
		ship.setScale(scale);
		TransparentShader.setMat4fv(ship.GetModelMat(), "model");
		//Light.setMat4fv("model", model);
		ourModel.Draw(TransparentShader);

		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		
		highlightshader.Use();
		
		highlightshader.setMat4fv(camera.GetViewMat(), "view");
		highlightshader.setMat4fv(camera.GetProjectionMat(), "projection");
		
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(270.f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, scale + scalediff);
		ship.setScale(scale + scalediff);
		highlightshader.setMat4fv(ship.GetModelMat(), "model");
		
		ourModel.Draw(highlightshader);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		
		
		
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		//// clear all relevant buffers
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT);
		//
		//AfterEffects.Use();
		//glBindVertexArray(quadVAO);
		//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		//glDrawArrays(GL_TRIANGLES, 0, 6);




		 
		//glm::mat4 model(1.0f);
		//model = glm::translate(model, glm::vec3((float)glfwGetTime(), 0.0f, (float)glfwGetTime()));
		//model = glm::rotate(model, glm::radians(270.f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));




		//cubeBox.Draw(SkyboxShader, camera);
		
		//glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		//SkyboxShader.Use();
		//glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMat())); // remove translation from the view matrix
		//SkyboxShader.setMat4fv(view, "view");
		//SkyboxShader.setMat4fv(camera.GetProjectionMat() ,"projection");
		//// skybox cube
		//glBindVertexArray(skyboxVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
		//glDepthFunc(GL_LESS);
		


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}