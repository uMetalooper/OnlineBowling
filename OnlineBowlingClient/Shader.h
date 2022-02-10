class Shader
{
public:
    Shader()
    {
        ID = 0;
    }
	Shader(const char* vertexPath, const char* fragmentPath);

    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, glm::mat4 matrix) const
    {
        GLuint loc = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, glm::vec3 vec) const
    {
        GLuint loc = glGetUniformLocation(ID, name.c_str());
        glUniform3f(loc, vec.x, vec.y, vec.z);
    }
private:
	void checkCompileErrors(unsigned int shader, std::string type);
	unsigned int ID;
};

