/* This file was automatically generated by CasADi.
   The CasADi copyright holders make no ownership claim of its contents. */
/* How to prefix internal symbols */
#ifdef CASADI_CODEGEN_PREFIX
  #define CASADI_NAMESPACE_CONCAT(NS, ID) _CASADI_NAMESPACE_CONCAT(NS, ID)
  #define _CASADI_NAMESPACE_CONCAT(NS, ID) NS ## ID
  #define CASADI_PREFIX(ID) CASADI_NAMESPACE_CONCAT(CODEGEN_PREFIX, ID)
#else
  #define CASADI_PREFIX(ID) eval_jac_g_leg_complex_to_simple_ ## ID
#endif

#include <math.h>

#ifndef casadi_real
#define casadi_real double
#endif

#ifndef casadi_int
#define casadi_int long long int
#endif

/* Add prefix to internal symbols */
#define casadi_f0 CASADI_PREFIX(f0)
#define casadi_s0 CASADI_PREFIX(s0)
#define casadi_s1 CASADI_PREFIX(s1)
#define casadi_s2 CASADI_PREFIX(s2)
#define casadi_sq CASADI_PREFIX(sq)

/* Symbol visibility in DLLs */
#ifndef CASADI_SYMBOL_EXPORT
  #if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    #if defined(STATIC_LINKED)
      #define CASADI_SYMBOL_EXPORT
    #else
      #define CASADI_SYMBOL_EXPORT __declspec(dllexport)
    #endif
  #elif defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
    #define CASADI_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define CASADI_SYMBOL_EXPORT
  #endif
#endif

casadi_real casadi_sq(casadi_real x) { return x*x;}

static const casadi_int casadi_s0[64] = {60, 1, 0, 60, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};
static const casadi_int casadi_s1[30] = {26, 1, 0, 26, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
static const casadi_int casadi_s2[212] = {28, 60, 0, 4, 8, 11, 16, 21, 24, 26, 28, 30, 34, 40, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 53, 60, 68, 75, 82, 90, 97, 104, 112, 119, 126, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 145, 147, 149, 0, 9, 10, 11, 1, 9, 10, 11, 2, 9, 10, 3, 4, 5, 10, 11, 3, 4, 5, 9, 11, 5, 9, 10, 0, 6, 1, 7, 2, 8, 3, 9, 10, 11, 3, 4, 5, 9, 10, 11, 3, 4, 5, 9, 10, 11, 0, 6, 9, 10, 11, 12, 13, 1, 7, 9, 10, 11, 14, 15, 2, 8, 9, 10, 12, 13, 14, 15, 0, 6, 9, 10, 11, 16, 17, 1, 7, 9, 10, 11, 18, 19, 2, 8, 9, 10, 16, 17, 18, 19, 0, 6, 9, 10, 11, 20, 21, 1, 7, 9, 10, 11, 22, 23, 2, 8, 9, 10, 20, 21, 22, 23, 0, 6, 9, 10, 11, 24, 25, 1, 7, 9, 10, 11, 26, 27, 2, 8, 9, 10, 24, 25, 26, 27, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 10, 11, 10, 11};

/* eval_jac_g_leg_complex_to_simple:(w[60],p[26])->(jac_g[28x60,149nz]) */
static int casadi_f0(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, int mem) {
  casadi_real a0, a1, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a2, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a3, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a4, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a5, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a6, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a7, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a8, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a9, a90, a91, a92, a93, a94, a95, a96, a97, a98;
  a0=-1.;
  if (res[0]!=0) res[0][0]=a0;
  a1=arg[0]? arg[0][4] : 0;
  a2=sin(a1);
  a3=arg[0]? arg[0][46] : 0;
  a4=(a2*a3);
  a5=arg[0]? arg[0][43] : 0;
  a6=(a2*a5);
  a4=(a4+a6);
  a6=arg[0]? arg[0][40] : 0;
  a7=(a2*a6);
  a4=(a4+a7);
  a7=arg[0]? arg[0][5] : 0;
  a8=sin(a7);
  a9=cos(a1);
  a10=arg[0]? arg[0][47] : 0;
  a11=(a9*a10);
  a12=arg[0]? arg[0][44] : 0;
  a13=(a9*a12);
  a14=(a11+a13);
  a15=arg[0]? arg[0][41] : 0;
  a16=(a9*a15);
  a14=(a14+a16);
  a17=arg[0]? arg[0][38] : 0;
  a18=(a9*a17);
  a14=(a14+a18);
  a19=(a8*a14);
  a4=(a4+a19);
  a19=arg[0]? arg[0][37] : 0;
  a20=(a2*a19);
  a4=(a4+a20);
  a4=(-a4);
  if (res[0]!=0) res[0][1]=a4;
  a4=cos(a7);
  a20=(a10+a12);
  a20=(a20+a15);
  a20=(a20+a17);
  a21=(a4*a20);
  a21=(-a21);
  if (res[0]!=0) res[0][2]=a21;
  a21=(a3+a5);
  a21=(a21+a6);
  a21=(a21+a19);
  if (res[0]!=0) res[0][3]=a21;
  if (res[0]!=0) res[0][4]=a0;
  a21=arg[0]? arg[0][45] : 0;
  a22=(a2*a21);
  a23=arg[0]? arg[0][42] : 0;
  a24=(a2*a23);
  a22=(a22+a24);
  a24=arg[0]? arg[0][39] : 0;
  a25=(a2*a24);
  a22=(a22+a25);
  a25=(a11+a13);
  a25=(a25+a16);
  a25=(a25+a18);
  a26=(a4*a25);
  a22=(a22+a26);
  a26=arg[0]? arg[0][36] : 0;
  a27=(a2*a26);
  a22=(a22+a27);
  if (res[0]!=0) res[0][5]=a22;
  a22=(a10+a12);
  a22=(a22+a15);
  a22=(a22+a17);
  a27=(a8*a22);
  a27=(-a27);
  if (res[0]!=0) res[0][6]=a27;
  a27=(a21+a23);
  a27=(a27+a24);
  a27=(a27+a26);
  a27=(-a27);
  if (res[0]!=0) res[0][7]=a27;
  if (res[0]!=0) res[0][8]=a0;
  a27=(a9*a4);
  a28=(a27*a3);
  a29=(a9*a8);
  a30=(a29*a21);
  a28=(a28-a30);
  a30=(a9*a4);
  a31=(a30*a5);
  a32=(a9*a8);
  a33=(a32*a23);
  a31=(a31-a33);
  a28=(a28+a31);
  a31=(a9*a4);
  a33=(a31*a6);
  a34=(a9*a8);
  a35=(a34*a24);
  a33=(a33-a35);
  a28=(a28+a33);
  a33=(a9*a4);
  a35=(a33*a19);
  a36=(a9*a8);
  a37=(a36*a26);
  a35=(a35-a37);
  a28=(a28+a35);
  a28=(-a28);
  if (res[0]!=0) res[0][9]=a28;
  a28=(a8*a3);
  a35=(a4*a21);
  a28=(a28+a35);
  a35=(a8*a5);
  a37=(a4*a23);
  a35=(a35+a37);
  a28=(a28+a35);
  a35=(a8*a6);
  a37=(a4*a24);
  a35=(a35+a37);
  a28=(a28+a35);
  a35=(a8*a19);
  a37=(a4*a26);
  a35=(a35+a37);
  a28=(a28+a35);
  if (res[0]!=0) res[0][10]=a28;
  a28=arg[0]? arg[0][3] : 0;
  a35=sin(a28);
  a37=arg[0]? arg[0][11] : 0;
  a38=(a2/a9);
  a39=(a37*a38);
  a39=(a35*a39);
  a40=cos(a28);
  a41=arg[0]? arg[0][10] : 0;
  a42=(a2/a9);
  a43=(a41*a42);
  a43=(a40*a43);
  a39=(a39-a43);
  a43=arg[1]? arg[1][0] : 0;
  a44=(1./a43);
  a39=(a39-a44);
  if (res[0]!=0) res[0][11]=a39;
  a39=(a40*a37);
  a45=(a35*a41);
  a39=(a39+a45);
  if (res[0]!=0) res[0][12]=a39;
  a39=(a37/a9);
  a39=(a35*a39);
  a45=(a41/a9);
  a45=(a40*a45);
  a39=(a39-a45);
  if (res[0]!=0) res[0][13]=a39;
  a39=22354293482292820.;
  a45=-1.3877787807814460e-17;
  a46=arg[0]? arg[0][9] : 0;
  a47=(a45*a46);
  a48=(a39*a47);
  a49=(a41*a48);
  a50=-4.8802931580173908e-01;
  a51=arg[0]? arg[0][59] : 0;
  a51=(a51-a37);
  a52=(a51/a43);
  a52=(a50*a52);
  a49=(a49+a52);
  a49=(a40*a49);
  a52=26445640661418040.;
  a47=(a52*a47);
  a53=(a37*a47);
  a54=4.3125046780173909e-01;
  a55=arg[0]? arg[0][58] : 0;
  a55=(a55-a41);
  a56=(a55/a43);
  a56=(a54*a56);
  a53=(a53+a56);
  a53=(a35*a53);
  a49=(a49-a53);
  if (res[0]!=0) res[0][14]=a49;
  a49=-3.6700698954086958e-01;
  a53=(a46*a37);
  a56=(a53*a9);
  a56=(a49*a56);
  a55=(a55/a43);
  a55=(a54*a55);
  a57=(a9*a55);
  a56=(a56+a57);
  a40=(a40*a56);
  a56=(a46*a41);
  a57=3.1022814154086947e-01;
  a58=(a57*a9);
  a59=(a56*a58);
  a60=4.8802931580173908e-01;
  a51=(a51/a43);
  a51=(a60*a51);
  a61=(a9*a51);
  a59=(a59+a61);
  a35=(a35*a59);
  a40=(a40-a35);
  if (res[0]!=0) res[0][15]=a40;
  a40=cos(a1);
  a35=sin(a28);
  a59=(a41*a35);
  a61=(a59/a9);
  a28=cos(a28);
  a62=(a37*a28);
  a63=(a62/a9);
  a61=(a61+a63);
  a61=(a40*a61);
  a1=sin(a1);
  a63=(a46*a9);
  a62=(a62*a2);
  a63=(a63+a62);
  a59=(a59*a2);
  a63=(a63+a59);
  a63=(a63/a9);
  a63=(a63/a9);
  a59=(a46/a9);
  a63=(a63-a59);
  a63=(a1*a63);
  a61=(a61+a63);
  a61=(-a61);
  if (res[0]!=0) res[0][16]=a61;
  a61=(1./a43);
  a63=(-a61);
  if (res[0]!=0) res[0][17]=a63;
  a63=(a37*a28);
  a59=(a41*a35);
  a63=(a63+a59);
  a63=(a63/a9);
  a63=(a63/a9);
  a63=(a1*a63);
  a63=(-a63);
  if (res[0]!=0) res[0][18]=a63;
  a63=arg[1]? arg[1][11] : 0;
  a59=arg[0]? arg[0][0] : 0;
  a62=(a63-a59);
  a64=(a62*a3);
  a65=arg[1]? arg[1][12] : 0;
  a66=arg[0]? arg[0][1] : 0;
  a67=(a65-a66);
  a68=(a67*a21);
  a64=(a64-a68);
  a68=arg[1]? arg[1][8] : 0;
  a69=(a68-a59);
  a70=(a69*a5);
  a64=(a64+a70);
  a70=arg[1]? arg[1][9] : 0;
  a71=(a70-a66);
  a72=(a71*a23);
  a64=(a64-a72);
  a72=arg[1]? arg[1][5] : 0;
  a73=(a72-a59);
  a74=(a73*a6);
  a64=(a64+a74);
  a74=arg[1]? arg[1][6] : 0;
  a75=(a74-a66);
  a76=(a75*a24);
  a64=(a64-a76);
  a76=arg[1]? arg[1][2] : 0;
  a77=(a76-a59);
  a78=(a77*a19);
  a64=(a64+a78);
  a78=arg[1]? arg[1][3] : 0;
  a79=(a78-a66);
  a80=(a79*a26);
  a64=(a64-a80);
  a64=(a40*a64);
  a80=(a66*a4);
  a81=(a59*a8);
  a82=(a80-a81);
  a83=(a65*a4);
  a82=(a82-a83);
  a83=(a63*a8);
  a82=(a82+a83);
  a83=(a82*a10);
  a84=arg[1]? arg[1][13] : 0;
  a85=arg[0]? arg[0][2] : 0;
  a84=(a84-a85);
  a86=(a84*a3);
  a87=(a4*a86);
  a83=(a83+a87);
  a87=(a84*a21);
  a88=(a8*a87);
  a83=(a83-a88);
  a88=(a80-a81);
  a89=(a70*a4);
  a88=(a88-a89);
  a89=(a68*a8);
  a88=(a88+a89);
  a89=(a88*a12);
  a83=(a83+a89);
  a89=arg[1]? arg[1][10] : 0;
  a89=(a89-a85);
  a90=(a89*a5);
  a91=(a4*a90);
  a83=(a83+a91);
  a91=(a89*a23);
  a92=(a8*a91);
  a83=(a83-a92);
  a92=(a80-a81);
  a93=(a74*a4);
  a92=(a92-a93);
  a93=(a72*a8);
  a92=(a92+a93);
  a93=(a92*a15);
  a83=(a83+a93);
  a93=arg[1]? arg[1][7] : 0;
  a93=(a93-a85);
  a94=(a93*a6);
  a95=(a4*a94);
  a83=(a83+a95);
  a95=(a93*a24);
  a96=(a8*a95);
  a83=(a83-a96);
  a80=(a80-a81);
  a81=(a78*a4);
  a80=(a80-a81);
  a81=(a76*a8);
  a80=(a80+a81);
  a81=(a80*a17);
  a83=(a83+a81);
  a81=arg[1]? arg[1][4] : 0;
  a81=(a81-a85);
  a85=(a81*a19);
  a96=(a4*a85);
  a83=(a83+a96);
  a96=(a81*a26);
  a97=(a8*a96);
  a83=(a83-a97);
  a83=(a1*a83);
  a64=(a64-a83);
  if (res[0]!=0) res[0][19]=a64;
  a64=-5.6778848000000048e-02;
  a83=(a41*a37);
  a83=(a64*a83);
  a97=-1.2102232626086960e-01;
  a98=arg[0]? arg[0][57] : 0;
  a98=(a98-a46);
  a98=(a98/a43);
  a98=(a97*a98);
  a83=(a83+a98);
  a40=(a40*a83);
  a49=(a49*a35);
  a53=(a53*a49);
  a56=(a56*a28);
  a57=(a57*a56);
  a53=(a53+a57);
  a51=(a28*a51);
  a53=(a53+a51);
  a55=(a35*a55);
  a53=(a53+a55);
  a1=(a1*a53);
  a40=(a40-a1);
  if (res[0]!=0) res[0][20]=a40;
  a40=(1./a43);
  a1=(-a40);
  if (res[0]!=0) res[0][21]=a1;
  a1=cos(a7);
  a53=(a63*a11);
  a87=(a9*a87);
  a53=(a53-a87);
  a87=(a68*a13);
  a53=(a53+a87);
  a91=(a9*a91);
  a53=(a53-a91);
  a91=(a72*a16);
  a53=(a53+a91);
  a95=(a9*a95);
  a53=(a53-a95);
  a95=(a76*a18);
  a53=(a53+a95);
  a14=(a59*a14);
  a53=(a53-a14);
  a96=(a9*a96);
  a53=(a53-a96);
  a53=(a1*a53);
  a7=sin(a7);
  a86=(a9*a86);
  a11=(a65*a11);
  a86=(a86-a11);
  a13=(a70*a13);
  a86=(a86-a13);
  a90=(a9*a90);
  a86=(a86+a90);
  a16=(a74*a16);
  a86=(a86-a16);
  a94=(a9*a94);
  a86=(a86+a94);
  a18=(a78*a18);
  a86=(a86-a18);
  a25=(a66*a25);
  a86=(a86+a25);
  a85=(a9*a85);
  a86=(a86+a85);
  a86=(a7*a86);
  a53=(a53-a86);
  if (res[0]!=0) res[0][22]=a53;
  a53=(a65*a10);
  a3=(a84*a3);
  a53=(a53-a3);
  a3=(a70*a12);
  a53=(a53+a3);
  a5=(a89*a5);
  a53=(a53-a5);
  a5=(a74*a15);
  a53=(a53+a5);
  a6=(a93*a6);
  a53=(a53-a6);
  a6=(a78*a17);
  a53=(a53+a6);
  a22=(a66*a22);
  a53=(a53-a22);
  a19=(a81*a19);
  a53=(a53-a19);
  a1=(a1*a53);
  a10=(a63*a10);
  a21=(a84*a21);
  a10=(a10-a21);
  a12=(a68*a12);
  a10=(a10+a12);
  a23=(a89*a23);
  a10=(a10-a23);
  a15=(a72*a15);
  a10=(a10+a15);
  a24=(a93*a24);
  a10=(a10-a24);
  a17=(a76*a17);
  a10=(a10+a17);
  a20=(a59*a20);
  a10=(a10-a20);
  a26=(a81*a26);
  a10=(a10-a26);
  a7=(a7*a10);
  a1=(a1-a7);
  if (res[0]!=0) res[0][23]=a1;
  a1=(-a43);
  if (res[0]!=0) res[0][24]=a1;
  if (res[0]!=0) res[0][25]=a0;
  a1=(-a43);
  if (res[0]!=0) res[0][26]=a1;
  if (res[0]!=0) res[0][27]=a0;
  a1=(-a43);
  if (res[0]!=0) res[0][28]=a1;
  if (res[0]!=0) res[0][29]=a0;
  if (res[0]!=0) res[0][30]=a0;
  a1=1.2102232626086960e-01;
  a1=(a1/a43);
  a7=(-a1);
  if (res[0]!=0) res[0][31]=a7;
  a7=(a37*a28);
  a52=(a52*a7);
  a7=(a41*a35);
  a39=(a39*a7);
  a52=(a52+a39);
  a45=(a45*a52);
  if (res[0]!=0) res[0][32]=a45;
  a49=(a9*a49);
  a45=(a37*a49);
  a58=(a28*a58);
  a52=(a41*a58);
  a45=(a45+a52);
  a97=(a97*a2);
  a97=(a97/a43);
  a45=(a45-a97);
  if (res[0]!=0) res[0][33]=a45;
  a42=(a35*a42);
  a42=(-a42);
  if (res[0]!=0) res[0][34]=a42;
  a42=(-a28);
  if (res[0]!=0) res[0][35]=a42;
  a42=(a35/a9);
  a42=(-a42);
  if (res[0]!=0) res[0][36]=a42;
  a42=5.6778848000000048e-02;
  a45=(a42*a37);
  if (res[0]!=0) res[0][37]=a45;
  a48=(a35*a48);
  a45=(a54*a28);
  a45=(a45/a43);
  a48=(a48-a45);
  if (res[0]!=0) res[0][38]=a48;
  a58=(a46*a58);
  a64=(a64*a2);
  a37=(a37*a64);
  a58=(a58+a37);
  a37=(a9*a35);
  a54=(a54*a37);
  a54=(a54/a43);
  a58=(a58-a54);
  if (res[0]!=0) res[0][39]=a58;
  a38=(a28*a38);
  a38=(-a38);
  if (res[0]!=0) res[0][40]=a38;
  if (res[0]!=0) res[0][41]=a35;
  a38=(a28/a9);
  a38=(-a38);
  if (res[0]!=0) res[0][42]=a38;
  a42=(a42*a41);
  if (res[0]!=0) res[0][43]=a42;
  a47=(a28*a47);
  a50=(a50*a35);
  a50=(a50/a43);
  a47=(a47-a50);
  if (res[0]!=0) res[0][44]=a47;
  a46=(a46*a49);
  a41=(a41*a64);
  a46=(a46+a41);
  a28=(a28*a9);
  a60=(a60*a28);
  a60=(a60/a43);
  a46=(a46-a60);
  if (res[0]!=0) res[0][45]=a46;
  a46=7.5187969924812026e-02;
  a28=5.0000000000000000e-01;
  a41=casadi_sq(a43);
  a64=(a28*a41);
  a64=(a46*a64);
  a64=(-a64);
  if (res[0]!=0) res[0][46]=a64;
  a49=(a46*a43);
  a49=(-a49);
  if (res[0]!=0) res[0][47]=a49;
  a79=(a2*a79);
  a36=(a36*a81);
  a79=(a79+a36);
  a79=(-a79);
  if (res[0]!=0) res[0][48]=a79;
  a79=(a4*a81);
  a79=(-a79);
  if (res[0]!=0) res[0][49]=a79;
  a79=(a66-a78);
  a79=(-a79);
  if (res[0]!=0) res[0][50]=a79;
  a79=1.;
  if (res[0]!=0) res[0][51]=a79;
  if (res[0]!=0) res[0][52]=a0;
  a36=(a28*a41);
  a36=(a46*a36);
  a36=(-a36);
  if (res[0]!=0) res[0][53]=a36;
  a47=(a46*a43);
  a47=(-a47);
  if (res[0]!=0) res[0][54]=a47;
  a35=(a2*a77);
  a33=(a33*a81);
  a35=(a35+a33);
  if (res[0]!=0) res[0][55]=a35;
  a81=(a8*a81);
  a81=(-a81);
  if (res[0]!=0) res[0][56]=a81;
  a77=(-a77);
  if (res[0]!=0) res[0][57]=a77;
  if (res[0]!=0) res[0][58]=a79;
  if (res[0]!=0) res[0][59]=a0;
  a28=(a28*a41);
  a28=(a46*a28);
  a28=(-a28);
  if (res[0]!=0) res[0][60]=a28;
  a46=(a46*a43);
  a46=(-a46);
  if (res[0]!=0) res[0][61]=a46;
  a80=(a9*a80);
  if (res[0]!=0) res[0][62]=a80;
  a59=(a59*a4);
  a80=(a66*a8);
  a43=(a59+a80);
  a76=(a76*a4);
  a43=(a43-a76);
  a78=(a78*a8);
  a43=(a43-a78);
  a43=(-a43);
  if (res[0]!=0) res[0][63]=a43;
  a43=arg[1]? arg[1][1] : 0;
  a78=(-a43);
  if (res[0]!=0) res[0][64]=a78;
  a78=(-a43);
  if (res[0]!=0) res[0][65]=a78;
  a78=(-a43);
  if (res[0]!=0) res[0][66]=a78;
  a78=(-a43);
  if (res[0]!=0) res[0][67]=a78;
  if (res[0]!=0) res[0][68]=a64;
  if (res[0]!=0) res[0][69]=a49;
  a75=(a2*a75);
  a34=(a34*a93);
  a75=(a75+a34);
  a75=(-a75);
  if (res[0]!=0) res[0][70]=a75;
  a75=(a4*a93);
  a75=(-a75);
  if (res[0]!=0) res[0][71]=a75;
  a75=(a66-a74);
  a75=(-a75);
  if (res[0]!=0) res[0][72]=a75;
  if (res[0]!=0) res[0][73]=a79;
  if (res[0]!=0) res[0][74]=a0;
  if (res[0]!=0) res[0][75]=a36;
  if (res[0]!=0) res[0][76]=a47;
  a75=(a2*a73);
  a31=(a31*a93);
  a75=(a75+a31);
  if (res[0]!=0) res[0][77]=a75;
  a93=(a8*a93);
  a93=(-a93);
  if (res[0]!=0) res[0][78]=a93;
  a73=(-a73);
  if (res[0]!=0) res[0][79]=a73;
  if (res[0]!=0) res[0][80]=a79;
  if (res[0]!=0) res[0][81]=a0;
  if (res[0]!=0) res[0][82]=a28;
  if (res[0]!=0) res[0][83]=a46;
  a92=(a9*a92);
  if (res[0]!=0) res[0][84]=a92;
  a92=(a59+a80);
  a72=(a72*a4);
  a92=(a92-a72);
  a74=(a74*a8);
  a92=(a92-a74);
  a92=(-a92);
  if (res[0]!=0) res[0][85]=a92;
  a92=(-a43);
  if (res[0]!=0) res[0][86]=a92;
  a92=(-a43);
  if (res[0]!=0) res[0][87]=a92;
  a92=(-a43);
  if (res[0]!=0) res[0][88]=a92;
  a92=(-a43);
  if (res[0]!=0) res[0][89]=a92;
  if (res[0]!=0) res[0][90]=a64;
  if (res[0]!=0) res[0][91]=a49;
  a71=(a2*a71);
  a32=(a32*a89);
  a71=(a71+a32);
  a71=(-a71);
  if (res[0]!=0) res[0][92]=a71;
  a71=(a4*a89);
  a71=(-a71);
  if (res[0]!=0) res[0][93]=a71;
  a71=(a66-a70);
  a71=(-a71);
  if (res[0]!=0) res[0][94]=a71;
  if (res[0]!=0) res[0][95]=a79;
  if (res[0]!=0) res[0][96]=a0;
  if (res[0]!=0) res[0][97]=a36;
  if (res[0]!=0) res[0][98]=a47;
  a71=(a2*a69);
  a30=(a30*a89);
  a71=(a71+a30);
  if (res[0]!=0) res[0][99]=a71;
  a89=(a8*a89);
  a89=(-a89);
  if (res[0]!=0) res[0][100]=a89;
  a69=(-a69);
  if (res[0]!=0) res[0][101]=a69;
  if (res[0]!=0) res[0][102]=a79;
  if (res[0]!=0) res[0][103]=a0;
  if (res[0]!=0) res[0][104]=a28;
  if (res[0]!=0) res[0][105]=a46;
  a88=(a9*a88);
  if (res[0]!=0) res[0][106]=a88;
  a88=(a59+a80);
  a68=(a68*a4);
  a88=(a88-a68);
  a70=(a70*a8);
  a88=(a88-a70);
  a88=(-a88);
  if (res[0]!=0) res[0][107]=a88;
  a88=(-a43);
  if (res[0]!=0) res[0][108]=a88;
  a88=(-a43);
  if (res[0]!=0) res[0][109]=a88;
  a88=(-a43);
  if (res[0]!=0) res[0][110]=a88;
  a88=(-a43);
  if (res[0]!=0) res[0][111]=a88;
  if (res[0]!=0) res[0][112]=a64;
  if (res[0]!=0) res[0][113]=a49;
  a67=(a2*a67);
  a29=(a29*a84);
  a67=(a67+a29);
  a67=(-a67);
  if (res[0]!=0) res[0][114]=a67;
  a67=(a4*a84);
  a67=(-a67);
  if (res[0]!=0) res[0][115]=a67;
  a66=(a66-a65);
  a66=(-a66);
  if (res[0]!=0) res[0][116]=a66;
  if (res[0]!=0) res[0][117]=a79;
  if (res[0]!=0) res[0][118]=a0;
  if (res[0]!=0) res[0][119]=a36;
  if (res[0]!=0) res[0][120]=a47;
  a2=(a2*a62);
  a27=(a27*a84);
  a2=(a2+a27);
  if (res[0]!=0) res[0][121]=a2;
  a84=(a8*a84);
  a84=(-a84);
  if (res[0]!=0) res[0][122]=a84;
  a62=(-a62);
  if (res[0]!=0) res[0][123]=a62;
  if (res[0]!=0) res[0][124]=a79;
  if (res[0]!=0) res[0][125]=a0;
  if (res[0]!=0) res[0][126]=a28;
  if (res[0]!=0) res[0][127]=a46;
  a9=(a9*a82);
  if (res[0]!=0) res[0][128]=a9;
  a59=(a59+a80);
  a63=(a63*a4);
  a59=(a59-a63);
  a65=(a65*a8);
  a59=(a59-a65);
  a59=(-a59);
  if (res[0]!=0) res[0][129]=a59;
  a59=(-a43);
  if (res[0]!=0) res[0][130]=a59;
  a59=(-a43);
  if (res[0]!=0) res[0][131]=a59;
  a59=(-a43);
  if (res[0]!=0) res[0][132]=a59;
  a43=(-a43);
  if (res[0]!=0) res[0][133]=a43;
  if (res[0]!=0) res[0][134]=a79;
  if (res[0]!=0) res[0][135]=a79;
  if (res[0]!=0) res[0][136]=a79;
  if (res[0]!=0) res[0][137]=a44;
  if (res[0]!=0) res[0][138]=a61;
  if (res[0]!=0) res[0][139]=a40;
  if (res[0]!=0) res[0][140]=a79;
  if (res[0]!=0) res[0][141]=a79;
  if (res[0]!=0) res[0][142]=a79;
  if (res[0]!=0) res[0][143]=a1;
  if (res[0]!=0) res[0][144]=a97;
  if (res[0]!=0) res[0][145]=a45;
  if (res[0]!=0) res[0][146]=a54;
  if (res[0]!=0) res[0][147]=a50;
  if (res[0]!=0) res[0][148]=a60;
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_leg_complex_to_simple(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, int mem){
  return casadi_f0(arg, res, iw, w, mem);
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_leg_complex_to_simple_alloc_mem(void) {
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_leg_complex_to_simple_init_mem(int mem) {
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT void eval_jac_g_leg_complex_to_simple_free_mem(int mem) {
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_leg_complex_to_simple_checkout(void) {
  return 0;
}

extern "C" CASADI_SYMBOL_EXPORT void eval_jac_g_leg_complex_to_simple_release(int mem) {
}

extern "C" CASADI_SYMBOL_EXPORT void eval_jac_g_leg_complex_to_simple_incref(void) {
}

extern "C" CASADI_SYMBOL_EXPORT void eval_jac_g_leg_complex_to_simple_decref(void) {
}

extern "C" CASADI_SYMBOL_EXPORT casadi_int eval_jac_g_leg_complex_to_simple_n_in(void) { return 2;}

extern "C" CASADI_SYMBOL_EXPORT casadi_int eval_jac_g_leg_complex_to_simple_n_out(void) { return 1;}

extern "C" CASADI_SYMBOL_EXPORT casadi_real eval_jac_g_leg_complex_to_simple_default_in(casadi_int i){
  switch (i) {
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT const char* eval_jac_g_leg_complex_to_simple_name_in(casadi_int i){
  switch (i) {
    case 0: return "w";
    case 1: return "p";
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT const char* eval_jac_g_leg_complex_to_simple_name_out(casadi_int i){
  switch (i) {
    case 0: return "jac_g";
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT const casadi_int* eval_jac_g_leg_complex_to_simple_sparsity_in(casadi_int i) {
  switch (i) {
    case 0: return casadi_s0;
    case 1: return casadi_s1;
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT const casadi_int* eval_jac_g_leg_complex_to_simple_sparsity_out(casadi_int i) {
  switch (i) {
    case 0: return casadi_s2;
    default: return 0;
  }
}

extern "C" CASADI_SYMBOL_EXPORT int eval_jac_g_leg_complex_to_simple_work(casadi_int *sz_arg, casadi_int* sz_res, casadi_int *sz_iw, casadi_int *sz_w) {
  if (sz_arg) *sz_arg = 2;
  if (sz_res) *sz_res = 1;
  if (sz_iw) *sz_iw = 0;
  if (sz_w) *sz_w = 0;
  return 0;
}


