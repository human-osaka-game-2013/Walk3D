xof 0303txt 0032

# X File Templates declaration.

template AnimTicksPerSecond
{
 <9e415a43-7ba6-4a73-8743-b73d47e88476>
 DWORD AnimTicksPerSecond;
}
template Frame
{
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}
template Matrix4x4
{
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}
template FrameTransformMatrix
{
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}
template Vector
{
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}
template Coords2d
{
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}
template ColorRGB
{
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}
template ColorRGBA
{
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}
template IndexedColor
{
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}
template MeshFace
{
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}
template MeshNormals
{
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}
template MeshVertexColors
{
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}
template MeshTextureCoords
{
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}
template Mesh
{
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}
template TextureFilename
{
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}
template Material
{
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}
template MeshMaterialList
{
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}
template VertexDuplicationIndices
{
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}
template XSkinMeshHeader
{
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}
template SkinWeights
{
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}
template Animation
{
 <3d82ab4f-62da-11cf-ab39-0020af71e433>
 [...]
}
template AnimationSet
{
 <3d82ab50-62da-11cf-ab39-0020af71e433>
 [Animation <3d82ab4f-62da-11cf-ab39-0020af71e433>]
}
template FloatKeys
{
 <10dd46a9-775b-11cf-8f52-0040333594a3>
 DWORD nValues;
 array FLOAT values[nValues];
}
template TimedFloatKeys
{
 <f406b180-7b3b-11cf-8f52-0040333594a3>
 DWORD time;
 FloatKeys tfkeys;
}
template AnimationKey
{
 <10dd46a8-775b-11cf-8f52-0040333594a3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}

# Global definition.


# Materials definition.

Material Material_1
{
 1.000000;1.000000;1.000000;1.000000;;
 5.000000;
 0.300000;0.300000;0.300000;;
 0.000000;0.000000;0.000000;;
 TextureFilename
 {
  "KEKKAI_TEX.png";
 }
}

# Models definition.

Frame Model
{
 FrameTransformMatrix
 {
  1.000000,0.000000,0.000000,0.000000,
  0.000000,1.000000,0.000000,0.000000,
  0.000000,0.000000,1.000000,0.000000,
  0.000000,0.000000,0.000000,1.000000;;
 }
 Frame Mesh_1
 {
  FrameTransformMatrix
  {
   100.000000,0.000000,0.000000,0.000000,
   0.000000,100.000000,0.000000,0.000000,
   0.000000,0.000000,100.000000,0.000000,
   0.000000,250.000000,0.000000,1.000000;;
  }
  Mesh
  {
   16;
   -30.000000;-2.500001;-30.000000;,
   30.000000;-2.500001;-30.000000;,
   -30.000000;2.500000;-30.000000;,
   30.000000;2.500000;-30.000000;,
   -30.000000;2.500000;30.000000;,
   30.000000;2.500000;30.000000;,
   -30.000000;-2.500001;30.000000;,
   30.000000;-2.500001;30.000000;,
   30.000000;-2.500001;-30.000000;,
   30.000000;-2.500001;30.000000;,
   30.000000;2.500000;30.000000;,
   30.000000;2.500000;-30.000000;,
   -30.000000;-2.500001;30.000000;,
   -30.000000;-2.500001;-30.000000;,
   -30.000000;2.500000;-30.000000;,
   -30.000000;2.500000;30.000000;;
   8;
   3;3,1,0;,
   3;2,3,0;,
   3;7,5,4;,
   3;6,7,4;,
   3;10,9,8;,
   3;11,10,8;,
   3;14,13,12;,
   3;15,14,12;;
   MeshNormals
   {
    16;
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;,
    -1.000000;0.000000;-0.000000;;
    8;
    3;3,1,0;,
    3;2,3,0;,
    3;7,5,4;,
    3;6,7,4;,
    3;10,9,8;,
    3;11,10,8;,
    3;14,13,12;,
    3;15,14,12;;
   }
   MeshTextureCoords
   {
    16;
    0.000000;1.000000;,
    1.000000;0.999999;,
    0.000000;0.000000;,
    1.000000;0.000000;,
    1.000000;0.000000;,
    0.000000;0.000001;,
    1.000000;1.000000;,
    0.000000;1.000000;,
    0.000000;1.000000;,
    1.000000;1.000000;,
    1.000000;0.000000;,
    0.000000;0.000000;,
    0.000000;1.000000;,
    1.000000;1.000000;,
    1.000000;0.000000;,
    0.000000;0.000000;;
   }
   MeshMaterialList
   {
    1;
    8;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;
    {Material_1}
   }
   VertexDuplicationIndices
   {
    16;
    8;
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    1,
    7,
    5,
    3,
    6,
    0,
    2,
    4;
   }
  }
 }
}

# Animations definition.

