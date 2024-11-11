#include "Object.h"

Object::Object(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec4>& colors,
    const std::vector<GLuint>& indices,
    const std::string& texFilePath_one,
    const std::string& texFilePath_two,
    const std::vector<glm::vec2>& texCoor
)
:indices_arr(indices)
{
    GLsizeiptr vertex_arr_size = vertices.size() * sizeof(glm::vec3);
    GLsizeiptr color_arr_size = colors.size() * sizeof(glm::vec4);
    GLsizeiptr index_arr_size = indices.size() * sizeof(GLuint);
    GLsizeiptr texCoor_size = texCoor.size() * sizeof(glm::vec2);

    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind VBO
    //VERTEX
    glGenBuffers(3, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertex_arr_size, vertices.data(), GL_STATIC_DRAW);
    // Position attribute - 3 components (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //COLOR
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, color_arr_size, colors.data(), GL_STATIC_DRAW);
    // Color attribute - 4 components (r, g, b, a)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1);

    //TEXTURES

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoor_size, texCoor.data(), GL_STATIC_DRAW);
    // Color attribute - 4 components (r, g, b, a)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(2);


    //Generate and bind EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Fill EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_arr_size, indices.data(), GL_STATIC_DRAW);


    //TEXTURE 1
    int width, height, channels;
    stbi_uc* tex_data = stbi_load(texFilePath_one.c_str(), &width, &height, &channels, 0);
    if (tex_data) {
        glGenTextures(1, &texture_one);
        glBindTexture(GL_TEXTURE_2D, texture_one);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }
    else {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }
    
    //TEXTURE 2
    tex_data = stbi_load(texFilePath_two.c_str(), &width, &height, &channels, 0);
    if (tex_data) {
        glGenTextures(1, &texture_two);
        glBindTexture(GL_TEXTURE_2D, texture_two);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }
    else {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }
    stbi_image_free(tex_data);

    glBindVertexArray(0);
}

void Object::draw() const
{
    //glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_one);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_two);
	glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_arr.size(), GL_UNSIGNED_INT, 0);
}

Object::~Object()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBO);
    glDeleteBuffers(1, &EBO);
    
}
