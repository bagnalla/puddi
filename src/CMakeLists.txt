include_directories(${puddi_SOURCE_DIR}/include)

add_library(puddi STATIC
	AnimatedObject.cc  Geometry.cc          Puddi.cc        Skeleton.cc
	Camera.cc          HeightMap.cc         Rectangle.cc    Skybox.cc
	CRSpline.cc        HeightMapTerrain.cc  RenderGraph.cc  Sphere.cc
	Cube.cc            InitShader.cc        RenderNode.cc   TerrainVertexMesh.cc
	DrawableObject.cc  LightSource.cc       Schematic.cc    Texture.cc
	EnvironmentMap.cc  Material.cc          SDLUtil.cc      UpdateGraph.cc
	Font.cc            ModelGraph.cc        Shader.cc       Util.cc
	FpsTracker.cc      Object.cc            Shadow.cc       VertexMesh.cc
        Line.cc
)

