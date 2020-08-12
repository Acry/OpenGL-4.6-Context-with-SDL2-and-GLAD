#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include "glad/glad.h"
#include "helper.h"

#define WW 550
#define WH (WW/16)*9
#define GL_GREY .5, .5, .5, 1
int ww=WW;
int wh=WH;

#define FRAG	"../assets/shader/frag/2.frag"
#define VERT	"../assets/shader/vert/2.vert"

GLuint shading_program;
GLuint vertexShader;
GLuint fragmentShader;

// loads a shader from file and returns the compiled shader
GLuint GetShader(GLenum, const char *);

//Get and build custom program from 2 files
GLuint custom_shaders(const char *, const char *);

//see https://github.com/Acry/C-Files
const char * read_file(const char *);

GLuint compile_shader(GLenum type, GLsizei, const char **);
GLuint program_check(GLuint);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    #pragma region setup sdl
    init();

    SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowSize(Window, ww, wh);
    SDL_SetWindowTitle(Window, "Context 4.6 with GLAD");
    SDL_ShowWindow(Window);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    #pragma endregion  setup sdl

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    glClearColor(GL_GREY);

    shading_program = custom_shaders(VERT, FRAG);
    glReleaseShaderCompiler();
    glUseProgram(shading_program);
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running =0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running =0;
                    break;
                default:
                    break;
                }
            }
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    glViewport(0, 0, event.window.data1, event.window.data2);
                }
            }
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glColor3f(0.0f, 0.0f, 0.0f);
        glRectf(-1.0, -1.0, 1.0, 1.0);
        SDL_GL_SwapWindow(Window);
    }

    // Clean up shaders
    //shader won't actually be deleted by glDeleteShader until it's been detached.
    glDetachShader(shading_program, vertexShader);
    glDetachShader(shading_program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shading_program);

    exit_();
    return EXIT_SUCCESS;
}

const char * read_file(const char *filename)
{
    long length = 0;
    char *result = NULL;
    FILE *file = fopen(filename, "r");
    if (file) {
        int status = fseek(file, 0, SEEK_END);
        if (status != 0) {
            fclose(file);
            return NULL;
        }
        length = ftell(file);
        status = fseek(file, 0, SEEK_SET);
        if (status != 0) {
            fclose(file);
            return NULL;
        }
        result = malloc((length+1) * sizeof(char));
        if (result) {
            size_t actual_length = fread(result, sizeof(char), length, file);
            result[actual_length++] = '\0';
        }
        fclose(file);
        return result;
    }
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't read %s", filename);
    return NULL;
}
#pragma region GPU PROGRAM CREATION
GLuint custom_shaders(const char *vsPath, const char *fsPath)
{
    vertexShader   = GetShader(GL_VERTEX_SHADER, vsPath);
    fragmentShader = GetShader(GL_FRAGMENT_SHADER, fsPath);

    shading_program = glCreateProgram();

    glAttachShader(shading_program, vertexShader);
    glAttachShader(shading_program, fragmentShader);

    glLinkProgram(shading_program);


    //Error Checking
    GLuint status;
    status=program_check(shading_program);
    if (status==GL_FALSE)
        return 0;
    return shading_program;

}

GLuint GetShader(GLenum eShaderType, const char *filename)
{

    const char *shaderSource=read_file(filename);
    GLuint shader = compile_shader(eShaderType, 1, &shaderSource);
    return shader;

}

GLuint compile_shader(GLenum type, GLsizei nsources, const char **sources)
{

    GLuint  shader;
    GLint   success, len;
    GLsizei i, srclens[nsources];

    for (i = 0; i < nsources; ++i)
        srclens[i] = (GLsizei)strlen(sources[i]);

    shader = glCreateShader(type);
    glShaderSource(shader, nsources, sources, srclens);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 1) {
            char *log;
            log = malloc(len);
            glGetShaderInfoLog(shader, len, NULL, log);
            fprintf(stderr, "%s\n\n", log);
            free(log);
        }
        SDL_Log("Error compiling shader.\n");
    }
    SDL_Log("shader: %u", shader);
    return shader;
}

GLuint program_check(GLuint program)
{
    //Error Checking
    GLint status;
    glValidateProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        if (len > 1) {
            char *log;
            log = malloc(len);
            glGetProgramInfoLog(program, len, &len, log);
            fprintf(stderr, "%s\n\n", log);
            free(log);
        }
        SDL_Log("Error linking shader default program.\n");
        return GL_FALSE;
    }
    return GL_TRUE;
}
#pragma endregion 	GPU PROGRAM CREATION
