#version 140

attribute vec4 vPosition;
attribute vec4 vNormal;
attribute vec2 vTextureCoordinate;
attribute vec4 vBoneIndices;
attribute vec4 vBoneWeights;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec2 fTextureCoord;
varying vec3 shadowCoordDepth;
varying vec3 vPositionLight;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 lightSource;
uniform vec4 cameraPosition;
uniform int shadowMode;
uniform mat4 lightProjection;
uniform samplerBuffer boneTransformTex;

void main()
{
    // SKINNING

	vec4 skinnedPos = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 skinnedNorm = vec4(0.0, 0.0, 0.0, 0.0);

	if (int(vBoneIndices.x) >= 0)
	{
        mat4 boneTransform = vBoneWeights.x * mat4(
            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 0),
            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 1),
            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 2),
            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 3));
        skinnedPos += boneTransform * vPosition;
        skinnedNorm += boneTransform * vNormal;

		if (int(vBoneIndices.y) >= 0)
        {
            mat4 boneTransform = vBoneWeights.y * mat4(
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 0),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 1),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 2),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 3));
            skinnedPos += boneTransform * vPosition;
            skinnedNorm += boneTransform * vNormal;
        }

        if (int(vBoneIndices.z) >= 0)
        {
            mat4 boneTransform = vBoneWeights.z * mat4(
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 0),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 1),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 2),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 3));
            skinnedPos += boneTransform * vPosition;
            skinnedNorm += boneTransform * vNormal;
        }

        if (int(vBoneIndices.w) >= 0)
        {
            mat4 boneTransform = vBoneWeights.w * mat4(
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 0),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 1),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 2),
                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 3));
            skinnedPos += boneTransform * vPosition;
            skinnedNorm += boneTransform * vNormal;
        }
	}
	else
	{
		skinnedPos = vPosition;
		skinnedNorm = vNormal;
	}

    // not sure if this works
//	mat4 boneTransform = mat4(0.0);
//
//	if (int(vBoneIndices.x) >= 0)
//	{
//        boneTransform += vBoneWeights.x * mat4(
//            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 0),
//            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 1),
//            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 2),
//            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 3));
//
//		if (int(vBoneIndices.y) >= 0)
//        {
//            boneTransform += vBoneWeights.y * mat4(
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 0),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 1),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 2),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 3));
//        }
//
//        if (int(vBoneIndices.z) >= 0)
//        {
//            boneTransform += vBoneWeights.z * mat4(
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 0),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 1),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 2),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 3));
//        }
//
//        if (int(vBoneIndices.w) >= 0)
//        {
//            boneTransform += vBoneWeights.w * mat4(
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 0),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 1),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 2),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 3));
//        }
//	}
//	else
//	{
//		boneTransform = mat4(1.0);
//	}
//
//	vec4 skinnedPos = boneTransform * vPosition;
//	vec4 skinnedNorm = boneTransform * vNormal;

	// END SKINNING

	// compute vPosition in world space
	vec4 vPositionWorld = model * skinnedPos;

	// compute normal in world space
	N = (model * skinnedNorm).xyz;

	// compute eye direction
	E = (cameraPosition - vPositionWorld).xyz;

	if (lightSource[3].w == 0.0)
		L = lightSource[3].xyz;
	else
		L = (lightSource[3] - vPositionWorld).xyz;

	if (shadowMode == 1)
	{
		vPositionLight = (lightProjection * vPositionWorld).xyz;
		float bias = 0.005*tan(acos(dot(normalize(N), normalize(L))));
		bias = clamp(bias, 0.0, 0.01);
		shadowCoordDepth = vec3((vPositionLight.x + 1.0) / 2.0, (vPositionLight.y + 1.0) / 2.0, (vPositionLight.z + 1.0) / 2.0 - bias);
	}

	// pass texture coordinates to be interpolated over fragments
	fTextureCoord = vec2((vTextureCoordinate.x), (vTextureCoordinate.y));

	// compute gl_Position
	gl_Position = projection * camera * vPositionWorld;
}
