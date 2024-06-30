void loadBMP(const char* imagepath, unsigned char** data, unsigned int* width, unsigned int* height) {

	// printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data

	// Open the file
	FILE* file = fopen(imagepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory?\n", imagepath);
		getchar();
		return;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	if (*(int*)&(header[0x1C]) != 24) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = (*width) * (*height) * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	*data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(*data, 1, imageSize, file);

	fprintf(stderr, "Done reading!\n");

	// Everything is in memory now, the file can be closed.
	fclose(file);

}


std::string readFile(std::string fileName) {
	std::ifstream myfile;
	myfile.open(fileName);

	std::string currentLine;

	std::string finalString;

	if (myfile.is_open()) {
		while (myfile.good()) {
			while (std::getline(myfile, currentLine)) {
				finalString += currentLine + "\n";
			}
		}
	}

	return finalString;
}

void checkShaderCompileStatus(GLint shaderID) {
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE) {
		// Compilation failed, retrieve and display compilation log
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* log = (GLchar*)malloc(logLength);
		glGetShaderInfoLog(shaderID, logLength, NULL, log);
		printf("Shader compilation failed:\n%s\n", log);
		free(log);
	}
}

class PlaneMesh {
	
	std::vector<GLfloat> verts;
	std::vector<GLfloat> normals;
	std::vector<unsigned int> indices;
	
	float time = 0;

	int min;
	int max;
	
	glm::vec4 modelColor;

	GLuint ProgramID;
	GLuint vaoID;
	GLuint MatrixID;
	GLuint indexBufferID;
	GLuint vertexPositionsBufferID;

	GLuint modelColorID;

	GLuint waterTextureID;
	GLuint displacementTextureID;


	void planeMeshQuads(float min, float max, float stepsize) {

		// The following coordinate system works as if (min, 0, min) is the origin
		// And then builds up the mesh from that origin down (in z)
		// and then to the right (in x).
		// So, one "row" of the mesh's vertices have a fixed x and increasing z

		//manually create a first column of vertices
		float x = min;
		float y = 0;
		for (float z = min; z <= max; z += stepsize) {
			verts.push_back(x);
			verts.push_back(y);
			verts.push_back(z);
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);
		}

		for (float x = min+stepsize; x <= max; x += stepsize) {
			for (float z = min; z <= max; z += stepsize) {
				verts.push_back(x);
				verts.push_back(y);
				verts.push_back(z);
				normals.push_back(0);
				normals.push_back(1);
				normals.push_back(0);
			}
		}

		int nCols = (max-min)/stepsize + 1;
		int i = 0, j = 0;
		for (float x = min; x < max; x += stepsize) {
			j = 0;
			for (float z = min; z < max; z += stepsize) {
				indices.push_back(i*nCols + j);
				indices.push_back(i*nCols + j + 1);
				indices.push_back((i+1)*nCols + j + 1);
				indices.push_back((i+1)*nCols + j);
				++j;
			}
			++i;
		}
	}

	public:

		PlaneMesh(float min, float max, float stepsize) {
			this->min = min;
			this->max = max;
			modelColor = glm::vec4(0, 1.0f, 1.0f, 1.0f);

			planeMeshQuads(min, max, stepsize);

			//gen and fill buffers
			//vertex attribute pointers
			//shaders and uniforms

			// Create the shaders
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint TessellationControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
			GLuint TessellationEvaluationShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
			GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			std::string VertexShaderCode = readFile("WaterVertexShader.glsl");

			std::string TessellationControlShaderCode = readFile("WaterTessellationControlShader.glsl");
			std::string TessellationEvaluationShaderCode = readFile("WaterTessellationEvaluationShader.glsl");

			/*std::cout << TessellationControlShaderCode << std::endl;
			std::cout << TessellationEvaluationShaderCode << std::endl;*/

			std::string GeometryShaderCode = readFile("WaterGeometryShader.glsl");
			std::string FragmentShaderCode = readFile("WaterFragmentShader.glsl");



			// Compile Vertex Shader
			char const* VertexSourcePointer = VertexShaderCode.c_str();
			glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
			glCompileShader(VertexShaderID);

			// Compile Tessellation Control Shader
			char const* TessellationControlSourcePointer = TessellationControlShaderCode.c_str();
			glShaderSource(TessellationControlShaderID, 1, &TessellationControlSourcePointer, NULL);
			glCompileShader(TessellationControlShaderID);

			// Compile Tessellation Evaluation Shader
			char const* TessellationEvaluationSourcePointer = TessellationEvaluationShaderCode.c_str();
			glShaderSource(TessellationEvaluationShaderID, 1, &TessellationEvaluationSourcePointer, NULL);
			glCompileShader(TessellationEvaluationShaderID);


			// Compile Geometry Shader
			char const* GeometrySourcePointer = GeometryShaderCode.c_str();
			glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
			glCompileShader(GeometryShaderID);


			// Compile Fragment Shader
			char const* FragmentSourcePointer = FragmentShaderCode.c_str();
			glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
			glCompileShader(FragmentShaderID);


			// Check compilation status
			ProgramID = glCreateProgram();

			glAttachShader(ProgramID, VertexShaderID);
			glAttachShader(ProgramID, TessellationControlShaderID);
			glAttachShader(ProgramID, TessellationEvaluationShaderID);
			glAttachShader(ProgramID, GeometryShaderID);
			glAttachShader(ProgramID, FragmentShaderID);
			glLinkProgram(ProgramID);


			checkShaderCompileStatus(VertexShaderID);
			checkShaderCompileStatus(TessellationControlShaderID);
			checkShaderCompileStatus(TessellationEvaluationShaderID);
			checkShaderCompileStatus(GeometryShaderID);
			checkShaderCompileStatus(FragmentShaderID);

			


			glDetachShader(ProgramID, VertexShaderID);
			glDetachShader(ProgramID, TessellationControlShaderID);
			glDetachShader(ProgramID, TessellationEvaluationShaderID);
			glDetachShader(ProgramID, GeometryShaderID);
			glDetachShader(ProgramID, FragmentShaderID);

			glDeleteShader(VertexShaderID);
			glDeleteShader(TessellationControlShaderID);
			glDeleteShader(TessellationEvaluationShaderID);
			glDeleteShader(GeometryShaderID);
			glDeleteShader(FragmentShaderID);



			glGenVertexArrays(1, &vaoID);
			glBindVertexArray(vaoID);
			
			unsigned char* data;
			GLuint width, height;

			std::string bitmapImageFilePath = "water.bmp";
			loadBMP(bitmapImageFilePath.c_str(), &data, &width, &height);
			glGenTextures(1, &waterTextureID);
			glActiveTexture(GL_TEXTURE0);


			glBindTexture(GL_TEXTURE_2D, waterTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);




			std::string bitmapImageFilePathDisplacement = "displacement-map1.bmp";
			loadBMP(bitmapImageFilePathDisplacement.c_str(), &data, &width, &height);
			glGenTextures(1, &displacementTextureID);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, displacementTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);



			MatrixID = glGetUniformLocation(ProgramID, "MVP");


			glGenBuffers(1, &vertexPositionsBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * verts.size(), verts.data(), GL_STATIC_DRAW);
			// 1st attribute buffer : Vertices
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0           // buffer offset
			);
			
			GLuint normalBufferID;
			glGenBuffers(1, &normalBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * normals.size(), normals.data(), GL_STATIC_DRAW);
			// 2nd attribute buffer : Normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0 			              // data
			);
			
			glGenBuffers(1, &indexBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
			
			glBindVertexArray(0); //good practice to unbind VAO when done specifying vertices


			std::cout << "# of Vertex floats: " << verts.size() << std::endl;
			std::cout << "# of Unique Vertices: " << verts.size() / 3 << std::endl;
			std::cout << "# of Normal floats: " << normals.size() << std::endl;
			std::cout << "# of Normals: " << normals.size() / 3 << std::endl;
			std::cout << "# of Indices/Vertices shown: " << indices.size() << std::endl;

		}

		void draw(glm::vec3 lightPos, glm::mat4 V, glm::mat4 P, glm::vec3 eye) {
			//Bind some stuff
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glm::mat4 M = glm::mat4(1.0f);
			glm::mat4 MVP = P * V * M;


			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, waterTextureID);
			
			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, displacementTextureID);


			glUseProgram(ProgramID);
			glBindVertexArray(vaoID);
			glPatchParameteri(GL_PATCH_VERTICES, 4);

			GLuint waterTextureID = glGetUniformLocation(ProgramID, "waterTexture");
			GLuint VID = glGetUniformLocation(ProgramID, "V");
			GLuint MID = glGetUniformLocation(ProgramID, "M");
			GLuint lightPositionID = glGetUniformLocation(ProgramID, "LightPosition_worldspace");

			GLuint timeID = glGetUniformLocation(ProgramID, "time");
			GLuint outerTessellationID = glGetUniformLocation(ProgramID, "outerTess");
			GLuint innerTessellationID = glGetUniformLocation(ProgramID, "innerTess");

			time += 0.01f;

			float tessValue = 64.0f;
			float outerTess = tessValue;
			float innerTess = tessValue;

			glUniform1f(timeID, time);
			glUniform1f(outerTessellationID, outerTess);
			glUniform1f(innerTessellationID, innerTess);


			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(VID, 1, GL_FALSE, &V[0][0]);
			glUniformMatrix4fv(MID, 1, GL_FALSE, &M[0][0]);
			glUniform3f(lightPositionID, 10.0f, 10.0f, 10.0f);

			vec3 CameraPosition_worldspace = eye;
			GLuint CameraPosition_worldspaceID = glGetUniformLocation(ProgramID, "CameraPosition_worldspaceID");

			glUniform3f(CameraPosition_worldspaceID, eye.x, eye.y, eye.z);

			glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, (void*)0);

			glBindVertexArray(0);
			glUseProgram(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
};
