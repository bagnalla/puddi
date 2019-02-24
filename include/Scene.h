#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "GlmHeaders.h"

namespace puddi
{
  class Camera;
  class Object;
  class RenderGraph;

  enum ProjectionType : int { PROJ_ORTHO, PROJ_PERSPECTIVE };

  class Scene
  {
  public:
    /* Scene(); */
    Scene(ProjectionType ptype, bool cam_object);

    // Deletes the camera associated with this scene.
    ~Scene();

    void SetProjectionType(ProjectionType pType);
    void SetRelativeWidth(float rel_width);
    void SetRelativeHeight(float rel_height);
    void SetDepth(float depth); // Only for ortho projection
    void SetFOV(float fov); // Only for perspective projection

    /* void SetProjection(const glm::mat4 &proj); */
    void SetView(const glm::mat4 &view);
    void SetCamera(Camera *cam);

    void ViewportChange(int w, int h);
    void ForceModelUpdate();

    /* void AddChildScene(Scene *child); */
    /* void RemoveChildScene(int child_id); */

    int GetId() const;
    glm::mat4 GetProjection() const;
    glm::mat4 GetView() const;
    Camera* GetCamera() const;
    /* std::vector<Scene*> GetChildScenes() const; */
    RenderGraph* GetRenderGraph() const;
    ProjectionType GetProjectionType() const;
    /* float GetRelativeWidth() const; */
    /* float GetRelativeHeight() const; */
    float GetDepth() const; // Only for ortho projection
    float GetFOV() const; // Only for perspective projection
    
    Object* NewObject();

    void Update();
    void Draw() const;

    // Transform a point in world space to view space.
    glm::vec4 UnprojectPoint(const glm::vec4 &p);

    // Transform a point in world space to viewport coordinates.
    glm::vec2 UnprojectPointVec2(const glm::vec4 &p);

    // Is a point in world space behind the camera? Nnot quite the
    // same as being within the camera's FOV. It's more like
    // partitioning the world space by a plane determined by the
    // camera's position and orientation, and checking which partition
    // the provided point is in.
    bool IsBehindEye(const glm::vec4 &p);

  private:
    int id;
    ProjectionType proj_type;
    float depth;
    float fov;
    /* float fov = static_cast<float>(M_PI / 4.0f); */
    mat4 projection;
    mat4 view;
    Camera *camera;
    RenderGraph *render_graph;
    Object *root;
    /* std::vector<Scene*> child_scenes; */
  };
}

#endif
