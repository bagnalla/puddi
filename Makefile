CC = g++
OPTIONS = -std=c++11 -fopenmp -O3
DEBUGOPTIONS = -std=c++11 -fopenmp -g
LDLIBS = -lGLEW -lGL -lSDL2 -lpng -ljpeg -lz -lSDL2_image -lassimp
RELEASEDIR = release
DEBUGDIR = debug
OBJDIR = obj
INCLUDEDIR = include
SRCDIR = src

releaseobjects = $(addprefix $(RELEASEDIR)/$(OBJDIR)/, Camera.o CRSpline.o Cube.o \
DrawableObject.o EnvironmentMap.o FpsTracker.o Puddi.o Geometry.o \
HeightMap.o HeightMapTerrain.o InitShader.o LightSource.o Material.o \
ModelGraph.o Object.o Rectangle.o RenderGraph.o RenderNode.o \
Schematic.o SDLUtil.o Shader.o Shadow.o Skybox.o Sphere.o \
TerrainVertexMesh.o Texture.o UpdateGraph.o Util.o VertexMesh.o Font.o \
SourceCode.o Token.o Lexer.o AST.o SyntaxParser.o )

debugobjects = $(addprefix $(DEBUGDIR)/$(OBJDIR)/, Camera.o CRSpline.o Cube.o \
DrawableObject.o EnvironmentMap.o FpsTracker.o Puddi.o Geometry.o \
HeightMap.o HeightMapTerrain.o InitShader.o LightSource.o Material.o \
ModelGraph.o Object.o Rectangle.o RenderGraph.o RenderNode.o \
Schematic.o SDLUtil.o Shader.o Shadow.o Skybox.o Sphere.o \
TerrainVertexMesh.o Texture.o UpdateGraph.o Util.o VertexMesh.o Font.o \
SourceCode.o Token.o Lexer.o AST.o SyntaxParser.o )

Release: $(SRCDIR)/main.cc releasedirs $(releaseobjects)
	$(CC) $(SRCDIR)/main.cc $(releaseobjects) -I$(INCLUDEDIR) $(OPTIONS) $(LDLIBS) -o $(RELEASEDIR)/puddi

Debug: $(SRCDIR)/main.cc debugdirs $(debugobjects)
	$(CC) $(SRCDIR)/main.cc $(debugobjects) -I$(INCLUDEDIR) $(DEBUGOPTIONS) $(LDLIBS) -o $(DEBUGDIR)/puddi

$(RELEASEDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CC) $< -c $(OPTIONS) -I$(INCLUDEDIR) $(LDLIBS) -o $@

$(DEBUGDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CC) $< -c $(DEBUGOPTIONS) -I$(INCLUDEDIR) $(LDLIBS) -o $@

releasedirs:
	mkdir -p $(RELEASEDIR)
	mkdir -p $(RELEASEDIR)/$(OBJDIR)

debugdirs:
	mkdir -p $(DEBUGDIR)
	mkdir -p $(DEBUGDIR)/$(OBJDIR)

clean: cleanRelease cleanDebug

cleanRelease:
	rm $(RELEASEDIR)/puddi $(releaseobjects)

cleanDebug:
	rm $(DEBUGDIR)/puddi $(debugobjects)
