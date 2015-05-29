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

AnimTicksPerSecond
{
 3000;
}

# Materials definition.


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
 Frame Camera_1
 {
  FrameTransformMatrix
  {
   -0.998451,0.055639,-0.000524,0.000000,
   0.055639,0.998451,0.000029,0.000000,
   0.000525,0.000000,-1.000000,0.000000,
   175.803467,25.841879,0.000000,1.000000;;
  }
 }
 Frame Null_2
 {
  FrameTransformMatrix
  {
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;
  }
 }
}

# Animations definition.

AnimationSet Animation_1
{
 Animation
 {
  {Camera_1}
  AnimationKey
  {
   4;
   37;
   100;16;
   -0.998451,0.055639,-0.000524,0.000000,
   0.055639,0.998451,0.000029,0.000000,
   0.000525,0.000000,-1.000000,0.000000,
   175.803467,25.841879,0.000000,1.000000;;,
   200;16;
   -0.999782,0.018556,-0.009525,0.000000,
   0.018727,0.999660,-0.018136,0.000000,
   0.009185,-0.018310,-0.999790,0.000000,
   175.803467,30.626228,3.882580,1.000000;;,
   300;16;
   -0.980748,-0.019359,-0.194315,0.000000,
   -0.018990,0.999813,-0.003763,0.000000,
   0.194351,-0.000000,-0.980932,0.000000,
   170.988815,39.911587,31.759079,1.000000;;,
   400;16;
   -0.864674,-0.084832,-0.495119,0.000000,
   -0.073617,0.996395,-0.042154,0.000000,
   0.496910,-0.000000,-0.867802,0.000000,
   146.928131,49.332199,81.617157,1.000000;;,
   500;16;
   -0.597619,-0.139534,-0.789546,0.000000,
   -0.084212,0.990217,-0.111257,0.000000,
   0.797346,-0.000000,-0.603523,0.000000,
   110.037476,59.645744,137.590546,1.000000;;,
   600;16;
   -0.098674,-0.197072,-0.975411,0.000000,
   -0.019835,0.980389,-0.196071,0.000000,
   0.994922,-0.000000,-0.100648,0.000000,
   22.338709,70.761505,174.652588,1.000000;;,
   700;16;
   0.397805,-0.253595,-0.881726,0.000000,
   0.104290,0.967311,-0.231157,0.000000,
   0.911523,0.000000,0.411249,0.000000,
   -70.073143,82.594276,164.393341,1.000000;;,
   800;16;
   0.837403,-0.319205,-0.443694,0.000000,
   0.282059,0.947686,-0.149448,0.000000,
   0.468187,-0.000000,0.883629,0.000000,
   -146.046951,95.068817,94.696732,1.000000;;,
   900;16;
   0.956030,0.274058,-0.104401,0.000000,
   -0.276059,0.961128,-0.004941,0.000000,
   0.098988,0.033544,0.994523,0.000000,
   -174.681763,108.112579,-11.144104,1.000000;;,
   1000;16;
   0.740543,-0.434315,0.512803,0.000000,
   0.357064,0.900761,0.247256,0.000000,
   -0.569300,-0.000000,0.822130,0.000000,
   -141.393646,121.655495,-102.034004,1.000000;;,
   1100;16;
   0.202046,-0.478546,0.854501,0.000000,
   0.110115,0.878062,0.465705,0.000000,
   -0.973166,-0.000000,0.230104,0.000000,
   -30.562584,135.624542,-178.090729,1.000000;;,
   1200;16;
   -0.617323,-0.514164,0.595439,0.000000,
   -0.370069,0.857692,0.356950,0.000000,
   -0.694234,-0.000000,-0.719749,0.000000,
   107.194572,149.949341,-138.387375,1.000000;;,
   1300;16;
   -0.833478,-0.549317,-0.059713,0.000000,
   -0.513739,0.730604,0.449767,0.000000,
   -0.203438,0.405548,-0.891148,0.000000,
   198.214569,164.566376,-6.494595,1.000000;;,
   1400;16;
   -0.731379,-0.530520,-0.428524,0.000000,
   -0.457738,0.847672,-0.268194,0.000000,
   0.505530,-0.000000,-0.862809,0.000000,
   209.329132,179.413208,136.209061,1.000000;;,
   1500;16;
   -0.432940,-0.552722,-0.712082,0.000000,
   -0.287143,0.833366,-0.472282,0.000000,
   0.854466,-0.000000,-0.519508,0.000000,
   130.457825,194.428375,204.900742,1.000000;;,
   1600;16;
   -0.022128,-0.579248,-0.814851,0.000000,
   -0.015724,0.815151,-0.579035,0.000000,
   0.999632,-0.000000,-0.027145,0.000000,
   11.206476,209.546417,243.082245,1.000000;;,
   1700;16;
   0.336612,-0.603638,-0.722712,0.000000,
   0.254863,0.797258,-0.547197,0.000000,
   0.906497,0.000000,0.422212,0.000000,
   -102.524849,224.702499,228.390884,1.000000;;,
   1800;16;
   0.664320,-0.638420,-0.388715,0.000000,
   0.551023,0.769688,-0.322420,0.000000,
   0.505028,-0.000001,0.863103,0.000000,
   -197.171875,239.836792,137.587982,1.000000;;,
   1900;16;
   0.986749,-0.079662,-0.141352,0.000000,
   0.071745,0.995601,-0.060256,0.000000,
   0.145531,0.049316,0.988124,0.000000,
   -235.291504,254.889420,1.656896,1.000000;;,
   2000;16;
   0.627739,-0.701678,0.337033,0.000000,
   0.618210,0.712494,0.331917,0.000000,
   -0.473033,-0.000000,0.881045,0.000000,
   -203.951843,269.800232,-112.718819,1.000000;;,
   2100;16;
   0.402512,-0.693186,0.597894,0.000000,
   0.387114,0.720759,0.575021,0.000000,
   -0.829534,0.000000,0.558456,0.000000,
   -135.797409,284.503693,-216.120071,1.000000;;,
   2200;16;
   0.075379,-0.695088,0.714962,0.000000,
   0.072880,0.718925,0.691257,0.000000,
   -0.994488,-0.000000,0.104849,0.000000,
   -21.551771,298.933258,-266.918579,1.000000;;,
   2300;16;
   -0.222133,-0.719278,0.658252,0.000000,
   -0.229986,0.694722,0.681519,0.000000,
   -0.947504,-0.000001,-0.319745,0.000000,
   88.535355,313.026031,-257.152832,1.000000;;,
   2400;16;
   -0.413058,-0.754814,0.509548,0.000000,
   -0.475323,0.655939,0.586355,0.000000,
   -0.776821,-0.000001,-0.629721,0.000000,
   166.042389,326.809631,-197.734985,1.000000;;,
   2500;16;
   -0.529021,-0.769776,0.357186,0.000000,
   -0.637974,0.638314,0.430749,0.000000,
   -0.559577,0.000000,-0.828779,0.000000,
   215.178772,339.943115,-141.440979,1.000000;;,
   2600;16;
   -0.615430,-0.768226,0.176283,0.000000,
   -0.738526,0.640179,0.211543,0.000000,
   -0.275366,-0.000000,-0.961340,0.000000,
   257.423615,348.279510,-78.305664,1.000000;;,
   2700;16;
   0.995834,-0.058166,0.070220,0.000000,
   -0.046267,-0.985943,-0.160548,0.000000,
   0.078572,0.156630,-0.984527,0.000000,
   266.601624,354.491699,1.363107,1.000000;;,
   2800;16;
   -0.600400,-0.777465,-0.187266,0.000000,
   -0.742200,0.628927,-0.231495,0.000000,
   0.297756,-0.000001,-0.954642,0.000000,
   251.665802,357.961700,92.169952,1.000000;;,
   2900;16;
   -0.422994,-0.795685,-0.433546,0.000000,
   -0.555661,0.605711,-0.569522,0.000000,
   0.715764,0.000000,-0.698343,0.000000,
   178.386658,359.381104,178.035904,1.000000;;,
   3000;16;
   -0.174895,-0.803527,-0.568995,0.000000,
   -0.236083,0.595268,-0.768063,0.000000,
   0.955864,-0.000001,-0.293809,0.000000,
   76.674545,360.817963,230.407318,1.000000;;,
   3100;16;
   0.276541,-0.802100,-0.529303,0.000000,
   0.371429,0.597190,-0.710918,0.000000,
   0.886321,-0.000000,0.463071,0.000000,
   -106.451851,362.240509,207.240646,1.000000;;,
   3200;16;
   0.545357,-0.802400,-0.242364,0.000000,
   0.733250,0.596787,-0.325867,0.000000,
   0.406115,0.000000,0.913822,0.000000,
   -213.363541,363.648102,127.183754,1.000000;;,
   3300;16;
   -0.987233,-0.040381,0.154078,0.000000,
   -0.011054,-0.947629,-0.319181,0.000000,
   0.158898,-0.316810,0.935085,0.000000,
   -226.221298,365.039398,-26.611988,1.000000;;,
   3400;16;
   0.445879,-0.790225,0.420400,0.000000,
   0.574960,0.612817,0.542104,0.000000,
   -0.686013,0.000000,0.727589,0.000000,
   -184.856415,366.403809,-188.630814,1.000000;;,
   3500;16;
   0.209449,-0.800748,0.561189,0.000000,
   0.279994,0.599001,0.750201,0.000000,
   -0.936875,0.000000,0.349665,0.000000,
   -81.120659,367.719147,-233.049026,1.000000;;,
   3600;16;
   -0.274268,-0.831548,0.483017,0.000000,
   -0.410596,0.555453,0.723107,0.000000,
   -0.869591,0.000000,-0.493773,0.000000,
   92.960228,368.944916,-191.808594,1.000000;;,
   3700;16;
   -0.164406,0.780712,-0.602876,0.000000,
   0.946982,0.295964,0.125022,0.000000,
   0.276036,-0.550358,-0.787979,0.000000,
   241.068542,369.835480,93.879387,1.000000;;;
  }
 }
 Animation
 {
  {Null_2}
  AnimationKey
  {
   4;
   37;
   100;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   200;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   300;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   400;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   500;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   600;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   700;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   800;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   900;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1000;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1100;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1200;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1300;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1400;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1500;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1600;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1700;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1800;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   1900;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2000;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2100;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2200;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2300;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2400;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2500;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2600;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2700;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2800;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   2900;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   3000;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   3100;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   3200;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   3300;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   3400;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   3500;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   3600;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;,
   3700;16;
   1.000000,0.000000,0.000000,0.000000,
   0.000000,1.000000,0.000000,0.000000,
   0.000000,0.000000,1.000000,0.000000,
   5.825584,35.313999,-0.089283,1.000000;;;
  }
 }
}
