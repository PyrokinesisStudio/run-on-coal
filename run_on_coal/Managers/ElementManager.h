#pragma once

namespace ROC
{

class Core;
class Animation;
class Camera;
class Collision;
class Element;
class File;
class Font;
class Geometry;
class Light;
class Model;
class Scene;
class RenderTarget;
class Shader;
class Sound;
class Texture;

class ElementManager final
{
    Core *m_core;

    std::unordered_set<void*> m_elementSet;
    std::unordered_set<void*>::iterator m_elementSetEnd;

    void AddElementToSet(void *f_ptr);
    void RemoveElementFromSet(void *f_ptr);

    ElementManager(const ElementManager &that);
    ElementManager& operator=(const ElementManager &that);
public:
    Scene* CreateScene();
    Camera* CreateCamera(int f_type);
    Light* CreateLight();
    Geometry* CreateGeometry(const std::string &f_path, bool f_async = false);
    Model* CreateModel(Geometry *f_geometry);
    Shader* CreateShader(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath);
    Animation* CreateAnimation(const std::string &f_path);
    Sound* CreateSound(const std::string &f_path);
    RenderTarget* CreateRenderTarget(int f_type, const glm::ivec2 &f_size, int f_filter);
    Texture* CreateTexture(const std::string &f_path, int f_type, int f_filter, bool f_compress);
    Texture* CreateTexture(const std::vector<std::string> &f_path, int f_filter, bool f_compress);
    Font* CreateFont_(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, int f_filter);
    File* CreateFile_(const std::string &f_path);
    File* OpenFile(const std::string &f_path, bool f_ro);
    Collision* CreateCollision(int f_type, const glm::vec3 &f_size, float f_mass);

    bool IsValidElement(void *f_ptr) const;
    bool DestroyElement(Element *f_element);
protected:
    explicit ElementManager(Core *f_core);
    ~ElementManager();

    friend class Core;
    friend class AsyncManager;
};

}
