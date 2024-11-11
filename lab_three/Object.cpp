#include "Object.h"

Object::Object(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec4>& colors,
    const std::vector<GLuint>& indices,
    const std::string& texFilePath,
    const std::vector<glm::vec2>& texCoor
)
:indices_arr(indices)
{
    GLsizeiptr vertex_arr_size = vertices.size() * sizeof(glm::vec3);
    GLsizeiptr color_arr_size = colors.size() * sizeof(glm::vec4);
    GLsizeiptr index_arr_size = indices.size() * sizeof(GLuint);

    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind VBO
    //VERTEX
    glGenBuffers(2, VBO);
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
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, color_arr_size, colors.data(), GL_STATIC_DRAW);
    // Color attribute - 4 components (r, g, b, a)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1);


    //Generate and bind EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Fill EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_arr_size, indices.data(), GL_STATIC_DRAW);

    int width, height, channels;
    stbi_uc* tex_data = stbi_load(texFilePath.c_str(), &width, &height, &channels, 0);
    if (tex_data) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tex_data);
    }
    else {
        std::cerr << "Error loading texture" << std::endl;
        exit(1);
    }
 


    glBindVertexArray(0);
}

void Object::draw() const
{
    glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_arr.size(), GL_UNSIGNED_INT, 0);
}

Object::~Object()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(3, VBO);
    glDeleteBuffers(1, &EBO);
    
}
