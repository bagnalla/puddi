CC = g++
OPTIONS = -std=c++11 -fopenmp -g -I/usr/include/freetype2 \
	-I/usr/local/include/oglft -I/usr/include/qt4 \
	-I/usr/include/qt4/Qt
LDLIBS = -lGLEW -lGL -lSDL2 -lpng -ljpeg -lz -lSDL2_image -lassimp \
	`freetype-config --libs` -lOGLFT
OBJDIR = obj
INCLUDEDIR = include
SRCDIR = src

objects = $(addprefix $(OBJDIR)/, Camera.o CRSpline.o Cube.o \
DrawableObject.o EnvironmentMap.o FpsTracker.o Puddi.o Geometry.o \
HeightMap.o HeightMapTerrain.o InitShader.o LightSource.o Material.o \
ModelGraph.o Object.o Rectangle.o RenderGraph.o RenderNode.o \
Schematic.o SDLUtil.o Shader.o Shadow.o Skybox.o Sphere.o \
TerrainVertexMesh.o Texture.o UpdateGraph.o Util.o VertexMesh.o Text.o \
Font.o TextString.o)

Release: $(SRCDIR)/main.cc $(objects)
	$(CC) $(SRCDIR)/main.cc $(objects) -I$(INCLUDEDIR) $(OPTIONS) $(LDLIBS) -o release/puddi

Debug: $(SRCDIR)/main.cc $(objects)
	$(CC) $(SRCDIR)/main.cc $(objects) -I$(INCLUDEDIR) $(OPTIONS) $(LDLIBS) -o debug/puddi

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CC) $< -c $(OPTIONS) -I$(INCLUDEDIR) $(LDLIBS) -o $@

clean: cleanRelease cleanDebug

cleanRelease:
	rm release/puddi $(objects)

cleanDebug:
	rm debug/puddi $(objects)
