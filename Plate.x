xof 0303txt 0032

template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}


Material ___________Default {
 0.784314;0.784314;0.784314;1.000000;;
 51.200001;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;
}

Material ___________Surface {
 0.815686;0.815686;0.815686;1.000000;;
 51.200001;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;
}

Frame Frame_SCENE_ROOT {

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame Frame1___________ {

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh Mesh___________ {
   4;
   -50.000000;0.000000;-50.000000;,
   50.000000;0.000000;-50.000000;,
   50.000000;0.000000;50.000000;,
   -50.000000;0.000000;50.000000;;
   2;
   3;0,3,2;,
   3;0,2,1;;

   MeshNormals {
    4;
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;;
    2;
    3;0,3,2;,
    3;0,2,1;;
   }

   MeshMaterialList {
    2;
    2;
    1,
    1;
    { ___________Default }
    { ___________Surface }
   }
  }
 }
}
