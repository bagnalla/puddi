attribute vec4 vPosition;
attribute vec4 vNormal;
attribute vec4 vTangent;
attribute vec4 vBinormal;
attribute vec2 vTextureCoordinate;
//attribute vec4 vBoneIndices;
//attribute vec4 vBoneWeights;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec2 fTextureCoord;
varying mat4 inverseTBN;
varying vec3 shadowCoordDepth;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 lightSource;
uniform vec4 cameraPosition;
uniform int shadowMode;
uniform mat4 lightProjection;
//uniform samplerBuffer boneTransformTex;

void main()
{
    // SKINNING
//
//	vec4 skinnedPos = vec4(0.0, 0.0, 0.0, 0.0);
//    vec4 skinnedNorm = vec4(0.0, 0.0, 0.0, 0.0);
//    vec4 skinnedBinorm = vec4(0.0, 0.0, 0.0, 0.0);
//    vec4 skinnedTan = vec4(0.0, 0.0, 0.0, 0.0);
//
//    if (int(vBoneIndices.x) >= 0)
//    {
//        // need to try just summing up the transforms and applying to vPosition at the end.
//        // it woud be faster I think but I'm not sure if it would actually work
//        mat4 boneTransform = vBoneWeights.x * mat4(
//            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 0),
//            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 1),
//            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 2),
//            texelFetch(boneTransformTex, 4 * int(vBoneIndices.x) + 3));
//        skinnedPos += boneTransform * vPosition;
//        skinnedNorm += boneTransform * vNormal;
//        skinnedBinorm += boneTransform * vBinormal;
//        skinnedTan += boneTransform * vTangent;
//
//        if (int(vBoneIndices.y) >= 0)
//        {
//            mat4 boneTransform = vBoneWeights.y * mat4(
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 0),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 1),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 2),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.y) + 3));
//            skinnedPos += boneTransform * vPosition;
//            skinnedNorm += boneTransform * vNormal;
//            skinnedBinorm += boneTransform * vBinormal;
//            skinnedTan += boneTransform * vTangent;
//        }
//
//        if (int(vBoneIndices.z) >= 0)
//        {
//            mat4 boneTransform = vBoneWeights.z * mat4(
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 0),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 1),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 2),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.z) + 3));
//            skinnedPos += boneTransform * vPosition;
//            skinnedNorm += boneTransform * vNormal;
//            skinnedBinorm += boneTransform * vBinormal;
//            skinnedTan += boneTransform * vTangent;
//        }
//
//        if (int(vBoneIndices.w) >= 0)
//        {
//            mat4 boneTransform = vBoneWeights.w * mat4(
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 0),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 1),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 2),
//                texelFetch(boneTransformTex, 4 * int(vBoneIndices.w) + 3));
//            skinnedPos += boneTransform * vPosition;
//            skinnedNorm += boneTransform * vNormal;
//            skinnedBinorm += boneTransform * vBinormal;
//            skinnedTan += boneTransform * vTangent;
//        }
//    }
//    else
//    {
//        skinnedPos = vPosition;
//        skinnedNorm = vNormal;
//    }

    // END SKINNING

    vec4 skinnedPos = vPosition;
    vec4 skinnedNorm = vNormal;

	// compute vPosition in world space
	vec4 vPositionWorld = model * skinnedPos;

	inverseTBN = mat4(vTangent, vBinormal, skinnedNorm, vec4(0.0, 0.0, 0.0, 0.0));

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
		vec3 vPositionLight = (lightProjection * vPositionWorld).xyz;
		float bias = 0.005*tan(acos(dot(normalize(N), normalize(L))));
		bias = clamp(bias, 0.0, 0.01);
		shadowCoordDepth = vec3((vPositionLight.x + 1.0) / 2.0, (vPositionLight.y + 1.0) / 2.0, (vPositionLight.z + 1.0) / 2.0 - bias);
	}

	// pass texture coordinates to be interpolated over fragments
	fTextureCoord = vec2((vTextureCoordinate.x), (vTextureCoordinate.y));

	// compute gl_Position
	gl_Position = projection * camera * vPositionWorld;
}
