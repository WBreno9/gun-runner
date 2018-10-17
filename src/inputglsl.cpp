#include "inputglsl.h"

GLuint GLSLShaderProgram(std::string vertFile, std::string fragFile)
{
        int status;
        char *errorLog;
        std::string vertProgram;
        std::string fragProgram;
        GLuint shaderProgram = 1;

        std::string str = "";
        std::ifstream fileStream;

        fileStream.open(vertFile);

        if (fileStream.fail()) {
                std::clog << "Failed to open vert file: " + vertFile
                          << std::endl;
                return 0;
        }

        while (!fileStream.eof()) {
                getline(fileStream, str);
                vertProgram += str + "\n";
        }
        fileStream.close();

        fileStream.open(fragFile);

        if (fileStream.fail()) {
                std::clog << "Failed to open frag file: " + fragFile
                          << std::endl;
                return 0;
        }

        while (!fileStream.eof()) {
                getline(fileStream, str);
                fragProgram += str + "\n";
        }
        fileStream.close();

        const GLchar* vertSource = (GLchar*)vertProgram.c_str();

        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &vertSource, NULL);
        glCompileShader(vertShader);

        glGetShaderiv(vertShader, GL_COMPILE_STATUS , &status);
        if(status == GL_FALSE)
        {
                errorLog = new char[512];
                glGetShaderInfoLog(vertShader, 512, NULL, errorLog);
                std::cout << errorLog << std::endl;
                delete errorLog;
        }

        const GLchar* fragSource = (GLchar*)fragProgram.c_str();;

        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragSource, NULL);
        glCompileShader(fragShader);

        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE)
        {
                errorLog = new char[512];
                glGetShaderInfoLog(fragShader, 512, NULL, errorLog);
                std::cout << errorLog << std::endl;
                delete errorLog;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertShader);
        glAttachShader(shaderProgram, fragShader);
        glBindFragDataLocation(shaderProgram, 0, "outColor");
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
        if(status == GL_FALSE)
        {
                errorLog = new char[512];
                glGetProgramInfoLog(fragShader, 512, NULL, errorLog);
                std::cout << errorLog << std::endl;
                delete errorLog;
        }  

        glDetachShader(shaderProgram, vertShader);
        glDetachShader(shaderProgram, fragShader);
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return shaderProgram;
}
