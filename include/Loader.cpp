#include <bits/stdc++.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class MyVertice {
  public:
    MyVertice(glm::vec4 c, glm::vec3 n, glm::vec2 t) {
            this->coords = c;
            this->normal = n;
            this->texture = t;
    }

    glm::vec4 getCoords() {
            return coords;
    }

    glm::vec3 getNormal() {
            return normal;
    }

    glm::vec2 getTexture() {
            return texture;
    }

private:
    glm::vec4 coords;
    glm::vec3 normal;
    glm::vec2 texture;
};

class Mesh {
  public:
    Mesh( const std::string &file_name ) {
            loadMesh(file_name);
    }

    std::vector<MyVertice> getMesh() {
            return vertices;
    }

    std::string getPath() {
            return path;
    }

    void loadMesh(const std::string &file_name ) {
            this->path = file_name;

            std::ifstream fin(file_name.c_str() );

            std::vector<MyVertice> vertices;

            if( !fin.fail() )
                    fin.close();
            else {
                    std::cerr << "Couldn't open file: " << file_name << std::endl;
                    return;
            }

            Assimp::Importer assimp_importer;
            const aiScene* assimp_scene_ = assimp_importer.ReadFile( file_name, 
                aiProcess_Triangulate | aiProcess_GenSmoothNormals);

            if( !assimp_scene_ ) {
                    std::cerr << assimp_importer.GetErrorString() << std::endl;
                    return;
            }

            if( assimp_scene_->HasMeshes() ) {
                    const aiVector3D zero(0.0f,0.0f,0.0f);

                    //Para cada modelo pegue um ponteiro para esse modelo
                    for( unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++ ) {
                            const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];
                            bool hasTexture = mesh_ptr->HasTextureCoords(0);

                            //para cada vertice do modelo pegue um ponteiro para esse vertice, 
                            //sua normal e suas coordenadas de textura
                            for( unsigned int vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id++ ) {
                                    const aiVector3D *vertex_ptr = &(mesh_ptr->mVertices[vertex_id]);
                                    const aiVector3D *normal_ptr = &(mesh_ptr->mNormals[vertex_id]);
                                    const aiVector3D *texture_ptr;
                                    if(hasTexture) {
                                            texture_ptr = &(mesh_ptr->mTextureCoords[0][vertex_id]);	
                                    } else {
                                            texture_ptr = &zero;
                                    }

                                    glm::vec4 coords( vertex_ptr->x, vertex_ptr->y, vertex_ptr->z,1 );
                                    glm::vec3 normal( normal_ptr->x, normal_ptr->y, normal_ptr->z );
                                    glm::vec2 texture (texture_ptr->x, texture_ptr->y );

                                    MyVertice v(coords, normal, texture);

                                    vertices.push_back(v);
                            }
                    }
            }

            this->vertices = vertices;
    }

  private:
    std::vector<MyVertice> vertices;
    std::string path;
};

std::vector<Mesh> modelos;

void display(void)
{
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, 512, 512);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(90.0f, 1.0f, 0.1f, 100.0f );
        gluLookAt(0.0f, 3.0f, 3.0f,
                        0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        std::vector<MyVertice> vertices = modelos[0].getMesh();

        for (int i = 0; i < vertices.size(); i+=3) {
                glBegin(GL_LINE_LOOP);
                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex3f(vertices[i].getCoords().x, vertices[i].getCoords().y, vertices[i].getCoords().z);

                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex3f(vertices[i+1].getCoords().x, vertices[i+1].getCoords().y, vertices[i+1].getCoords().z);

                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex3f(vertices[i+2].getCoords().x, vertices[i+2].getCoords().y, vertices[i+2].getCoords().z);
                glEnd();
        }

        glutSwapBuffers();
        glutPostRedisplay();
}

void free_mem(void)
{
        std::clog << "Exiting...\n";
}

//---------------------------------------------------------------
int main(int argc, char **argv)
{

        std::clog << "Begin...\n";
        //Iniciação glut
        glutInit(&argc,argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
        glutInitWindowSize(512, 512);
        glutInitWindowPosition(100,100);
        glutCreateWindow("OpenGL: Pipeline Inspection");

        //Enables
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHT0);

        //Componentes da luz
        float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        float diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
        float specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        float position[] = { -3.5f, 1.0f, 2.0f, 1.0f };
        //Definindo GL_LITGH0
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
        glLightfv(GL_LIGHT0, GL_POSITION, position);

        //Careegando as malhas
        Mesh suzanne("Modelos//Suzanne.obj");
        modelos.push_back(suzanne);
        Mesh esfera("Modelos//Esfera.obj");
        modelos.push_back(esfera);

        glutDisplayFunc(display);

        atexit(free_mem);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glutMainLoop();

        return 0;
}
