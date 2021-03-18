#ifndef __GENERATORS_H__
#define __GENERATORS_H__
#include <string>

class Generator32 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, a25, a26, a27, a28, a29, a30;
  int a31, a32, valmax;

public:
  Generator32()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0), a25(0), a26(0), a27(0), a28(0),
        a29(0), a30(0), a32(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a19],
        moves[a20], moves[a21], moves[a22], moves[a23], moves[a24], moves[a25],
        moves[a26], moves[a27], moves[a28], moves[a29], moves[a30], moves[a31],
        moves[a32]};
  }

  bool operator++() {
    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&  
		a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a32 < valmax ) {
      a32++;
      return true;
    }

    if (a32 == valmax && a31 < valmax) {
      a32 = 0;
      a31++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 < valmax) {
      a32 = 0;
      a31 = 0;
      a30++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && 
		a4 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&         a3 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
		a3 == valmax && a2 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a32 == valmax && a31 == valmax && a30 == valmax && a29 == valmax &&
        a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a32 = 0;
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator31 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, a25, a26, a27, a28, a29, a30;
  int a31, valmax;

public:
  Generator31()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0), a25(0), a26(0), a27(0), a28(0),
        a29(0), a30(0), a31(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a19],
        moves[a20], moves[a21], moves[a22], moves[a23], moves[a24], moves[a25],
        moves[a26], moves[a27], moves[a28], moves[a29], moves[a30], moves[a31]};
  }

  bool operator++() {
    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }
	if (a31 < valmax ) {
      a31++;
      return true;
    }

    if (a31 == valmax && a30 < valmax) {
      a31 = 0;
      a30++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 < valmax) {
      a31 = 0;
      a30 = 0;
      a29++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a31 == valmax && a30 == valmax && a29 == valmax && a28 == valmax &&
        a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a31 = 0;
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator30 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, valmax;

public:
  Generator30()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0), a25(0), a26(0), a27(0), a28(0),
        a29(0), a30(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a19],
        moves[a20], moves[a21], moves[a22], moves[a23], moves[a24], moves[a25],
        moves[a26], moves[a27], moves[a28], moves[a29], moves[a30]};
  }

  bool operator++() {
    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 == valmax) {
      return false;
    }
	
    if (a30 < valmax) {
      a30++;
      return true;
    }
	
    if (a30 == valmax && a29 < valmax) {
      a30 = 0;
      a29++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 < valmax) {
      a30 = 0;
      a29 = 0;
      a28++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a30 == valmax && a29 == valmax && a28 == valmax && a27 == valmax &&
        a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 < valmax) {
      a30 = 0;
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator29 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, a25, a26, a27, a28, a29, valmax;

public:
  Generator29()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0), a25(0), a26(0), a27(0), a28(0),
        a29(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18],
        moves[a19], moves[a20], moves[a21], moves[a22], moves[a23], moves[a24],
        moves[a25], moves[a26], moves[a27], moves[a28], moves[a29]};
  }

  bool operator++() {
    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 == valmax) {
      return false;
    }
	
    if (a29 < valmax ) {
      a29++;
      return true;
    }
	
    if (a29 == valmax && a28 < valmax) {
      a29 = 0;
      a28++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 < valmax) {
      a29 = 0;
      a28 = 0;
      a27++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a29 == valmax && a28 == valmax && a27 == valmax && a26 == valmax &&
        a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 < valmax) {
      a29 = 0;
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator28 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, a25, a26, a27, a28, valmax;

public:
  Generator28()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0), a25(0), a26(0), a27(0), a28(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18],
        moves[a19], moves[a20], moves[a21], moves[a22], moves[a23], moves[a24],
        moves[a25], moves[a26], moves[a27], moves[a28]};
  }

  bool operator++() {
    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }
	
	if (a28 < valmax ) {
      a28++;
      return true;
    }
	
    if (a28 == valmax && a27 < valmax) {
      a28 = 0;
      a27++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 < valmax) {
      a28 = 0;
      a27 = 0;
      a26++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 < valmax) {

      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && 
		a4 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
		a3 == valmax && a2 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a28 == valmax && a27 == valmax && a26 == valmax && a25 == valmax &&
        a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a28 = 0;
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator27 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, a25, a26, a27, valmax;

public:
  Generator27()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0), a25(0), a26(0), a27(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18],
        moves[a19], moves[a20], moves[a21], moves[a22], moves[a23], moves[a24],
        moves[a25], moves[a26], moves[a27]};
  }

  bool operator++() {
    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a27 < valmax) {
      a27++;
      return true;
    }

    if (a27 == valmax && a26 < valmax) {
      a27 = 0;
      a26++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 < valmax) {
      a27 = 0;
      a26 = 0;
      a25++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a27 == valmax && a26 == valmax && a25 == valmax && a24 == valmax &&
        a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a27 = 0;
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator26 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, a25, a26, valmax;

public:
  Generator26()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0), a25(0), a26(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18],
        moves[a19], moves[a20], moves[a21], moves[a22], moves[a23], moves[a24],
        moves[a25], moves[a26]};
  }

  bool operator++() {
    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a26 < valmax ) {
      a26++;
      return true;
    }

    if (a26 == valmax && a25 < valmax) {
      a26 = 0;
      a25++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 < valmax) {
      a26 = 0;
      a25 = 0;
      a24++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a26 == valmax && a25 == valmax && a24 == valmax && a23 == valmax &&
        a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 < valmax) {
      a26 = 0;
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator25 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, a25, valmax;

public:
  Generator25()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0), a25(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],
        moves[a6],  moves[a7],  moves[a8],  moves[a9],  moves[a10],
        moves[a11], moves[a12], moves[a13], moves[a14], moves[a15],
        moves[a16], moves[a17], moves[a19], moves[a20], moves[a21],
        moves[a22], moves[a23], moves[a24], moves[a18], moves[a25]};
  }

  bool operator++() {
    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 == valmax) {
      return false;
    }

    if (a25 < valmax) {
      a25++;
      return true;
    }

    if (a25 == valmax && a24 < valmax) {
      a25 = 0;
      a24++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 < valmax) {
      a25 = 0;
      a24 = 0;
      a23++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a25 == valmax && a24 == valmax && a23 == valmax && a22 == valmax &&
        a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 < valmax) {
      a25 = 0;
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator24 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, a24, valmax;

public:
  Generator24()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0), a24(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18],
        moves[a19], moves[a20], moves[a21], moves[a22], moves[a23], moves[a24]};
  }

  bool operator++() {
    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
		a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a24 < valmax ) {
      a24++;
      return true;
    }

    if (a24 == valmax && a23 < valmax) {
      a24 = 0;
      a23++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 < valmax) {
      a24 = 0;
      a23 = 0;
      a22++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&        a4 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&  
		a3 == valmax && a2 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a24 == valmax && a23 == valmax && a22 == valmax && a21 == valmax &&
        a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
		a3 == valmax && a2 == valmax && a1 < valmax) {
      a24 = 0;
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator23 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, a23, valmax;

public:
  Generator23()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0), a23(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18],
        moves[a19], moves[a20], moves[a21], moves[a22], moves[a23]};
  }

  bool operator++() {
    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a23 < valmax ) {
      a23++;
      return true;
    }

    if (a23 == valmax && a22 < valmax) {
      a23 = 0;
      a22++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 < valmax) {
      a23 = 0;
      a22 = 0;
      a21++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a23 == valmax && a22 == valmax && a21 == valmax && a20 == valmax &&
        a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a23 = 0;
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator22 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, a22, valmax;

public:
  Generator22()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0), a22(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18],
        moves[a19], moves[a20], moves[a21], moves[a22]};
  }

  bool operator++() {
    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a22 < valmax ) {
      a22++;
      return true;
    }

    if (a22 == valmax && a21 < valmax) {
      a22 = 0;
      a21++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 < valmax) {
      a22 = 0;
      a21 = 0;
      a20++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a22 == valmax && a21 == valmax && a20 == valmax && a19 == valmax &&
        a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 < valmax) {
      a22 = 0;
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator21 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int a21, valmax;

public:
  Generator21()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0), a21(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18],
        moves[a19], moves[a20], moves[a21]};
  }

  bool operator++() {
    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 == valmax) {
      return false;
    }

    if (a21 < valmax ) {
      a21++;
      return true;
    }

    if (a21 == valmax && a20 < valmax) {
      a21 = 0;
      a20++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 < valmax) {
      a21 = 0;
      a20 = 0;
      a19++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a21 == valmax && a20 == valmax && a19 == valmax && a18 == valmax &&
        a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 < valmax) {
      a21 = 0;
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator20 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19, a20;
  int valmax;

public:
  Generator20()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0),
        a20(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1],  moves[a2],  moves[a3],  moves[a4],
                       moves[a5],  moves[a6],  moves[a7],  moves[a8],
                       moves[a9],  moves[a10], moves[a11], moves[a12],
                       moves[a13], moves[a14], moves[a15], moves[a16],
                       moves[a17], moves[a18], moves[a19], moves[a20]};
  }

  bool operator++() {
    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a20 < valmax ) {
      a20++;
      return true;
    }

    if (a20 == valmax && a19 < valmax) {
      a20 = 0;
      a19++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 < valmax) {
      a20 = 0;
      a19 = 0;
      a18++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&        a4 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
		a3 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a20 == valmax && a19 == valmax && a18 == valmax && a17 == valmax &&
        a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
		a3 == valmax && a2 == valmax && a1 < valmax) {
      a20 = 0;
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator19 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18, a19;
  int valmax;

public:
  Generator19()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0), a19(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1],  moves[a2],  moves[a3],  moves[a4],
                       moves[a5],  moves[a6],  moves[a7],  moves[a8],
                       moves[a9],  moves[a10], moves[a11], moves[a12],
                       moves[a13], moves[a14], moves[a15], moves[a16],
                       moves[a17], moves[a18], moves[a19]};
  }

  bool operator++() {
    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a19 < valmax) {
      a19++;
      return true;
    }

    if (a19 == valmax && a18 < valmax) {
      a19 = 0;
      a18++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 < valmax) {
      a19 = 0;
      a18 = 0;
      a17++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a19 == valmax && a18 == valmax && a17 == valmax && a16 == valmax &&
        a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a19 = 0;
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator18 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17, a18;
  int valmax;

public:
  Generator18()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0), a18(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17], moves[a18]};
  }

  bool operator++() {
    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a18 < valmax) {
      a18++;
      return true;
    }

    if (a18 == valmax && a17 < valmax) {
      a18 = 0;
      a17++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 < valmax) {
      a18 = 0;
      a17 = 0;
      a16++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a18 == valmax && a17 == valmax && a16 == valmax && a15 == valmax &&
        a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 < valmax) {
      a18 = 0;
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator17 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16, a17;
  int valmax;

public:
  Generator17()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0), a17(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{
        moves[a1],  moves[a2],  moves[a3],  moves[a4],  moves[a5],  moves[a6],
        moves[a7],  moves[a8],  moves[a9],  moves[a10], moves[a11], moves[a12],
        moves[a13], moves[a14], moves[a15], moves[a16], moves[a17]};
  }

  bool operator++() {
    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 == valmax) {
      return false;
    }

    if (a17 < valmax ) {
      a17++;
      return true;
    }

    if (a17 == valmax && a16 < valmax) {
      a17 = 0;
      a16++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 < valmax) {
      a17 = 0;
      a16 = 0;
      a15++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a17 == valmax && a16 == valmax && a15 == valmax && a14 == valmax &&
        a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 < valmax) {
      a17 = 0;
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator16 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15, a16;
  int valmax;

public:
  Generator16()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0), a16(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1],  moves[a2],  moves[a3],  moves[a4],
                       moves[a5],  moves[a6],  moves[a7],  moves[a8],
                       moves[a9],  moves[a10], moves[a11], moves[a12],
                       moves[a13], moves[a14], moves[a15], moves[a16]};
  }

  bool operator++() {
    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a16 < valmax) {
      a16++;
      return true;
    }

    if (a16 == valmax && a15 < valmax) {
      a16 = 0;
      a15++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 < valmax) {
      a16 = 0;
      a15 = 0;
      a14++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&        a4 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a16 == valmax && a15 == valmax && a14 == valmax && a13 == valmax &&
        a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
		a3 == valmax && a2 == valmax && a1 < valmax) {
      a16 = 0;
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator15 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14, a15;
  int valmax;

public:
  Generator15()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0), a15(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1],  moves[a2],  moves[a3],  moves[a4],
                       moves[a5],  moves[a6],  moves[a7],  moves[a8],
                       moves[a9],  moves[a10], moves[a11], moves[a12],
                       moves[a13], moves[a14], moves[a15]};
  }

  bool operator++() {
    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a15 < valmax ) {
      a15++;
      return true;
    }

    if (a15 == valmax && a14 < valmax) {
      a15 = 0;
      a14++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 < valmax) {
      a15 = 0;
      a14 = 0;
      a13++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a15 == valmax && a14 == valmax && a13 == valmax && a12 == valmax &&
        a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a15 = 0;
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator14 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13, a14;
  int valmax;

public:
  Generator14()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0), a14(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1],  moves[a2],  moves[a3],  moves[a4],
                       moves[a5],  moves[a6],  moves[a7],  moves[a8],
                       moves[a9],  moves[a10], moves[a11], moves[a12],
                       moves[a13], moves[a14]};
  }

  bool operator++() {
    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax &&
        a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a14 < valmax) {
      a14++;
      return true;
    }

    if (a14 == valmax && a13 < valmax) {
      a14 = 0;
      a13++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 < valmax) {
      a14 = 0;
      a13 = 0;
      a12++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a14 == valmax && a13 == valmax && a12 == valmax && a11 == valmax &&
        a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 < valmax) {
      a14 = 0;
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator13 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12, a13;
  int valmax;

public:
  Generator13()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0), a13(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1],  moves[a2],  moves[a3], moves[a4], moves[a5],
                       moves[a6],  moves[a7],  moves[a8], moves[a9], moves[a10],
                       moves[a11], moves[a12], moves[a13]};
  }

  bool operator++() {
    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 == valmax) {
      return false;
    }

    if (a13 < valmax ) {
      a13++;
      return true;
    }

    if (a13 == valmax && a12 < valmax) {
      a13 = 0;
      a12++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 < valmax) {
      a13 = 0;
      a12 = 0;
      a11++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a13 == valmax && a12 == valmax && a11 == valmax && a10 == valmax &&
        a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 < valmax) {
      a13 = 0;
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator12 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11, a12;
  int valmax;

public:
  Generator12()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0), a12(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2],  moves[a3],  moves[a4],
                       moves[a5], moves[a6],  moves[a7],  moves[a8],
                       moves[a9], moves[a10], moves[a11], moves[a12]};
  }

  bool operator++() {
    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a12 < valmax ) {
      a12++;
      return true;
    }

    if (a12 == valmax && a11 < valmax) {
      a12 = 0;
      a11++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 < valmax) {
      a12 = 0;
      a11 = 0;
      a10++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&        a4 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a12 == valmax && a11 == valmax && a10 == valmax && a9 == valmax &&
        a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
		a3 == valmax && a2 == valmax && a1 < valmax) {
      a12 = 0;
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator11 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int a11;
  int valmax;

public:
  Generator11()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0),
        a11(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2],  moves[a3], moves[a4],
                       moves[a5], moves[a6],  moves[a7], moves[a8],
                       moves[a9], moves[a10], moves[a11]};
  }

  bool operator++() {
    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a11 < valmax ) {
      a11++;
      return true;
    }

    if (a11 == valmax && a10 < valmax) {
      a11 = 0;
      a10++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 < valmax) {
      a11 = 0;
      a10 = 0;
      a9++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 < valmax) {
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 < valmax) {
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 < valmax) {
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 < valmax) {
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 < valmax) {
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 < valmax) {
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 < valmax) {
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a11 == valmax && a10 == valmax && a9 == valmax && a8 == valmax &&
        a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a11 = 0;
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator10 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
  int valmax;

public:
  Generator10()
      : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0), a10(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2], moves[a3], moves[a4], moves[a5],
                       moves[a6], moves[a7], moves[a8], moves[a9], moves[a10]};
  }

  bool operator++() {
    if (a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a10 < valmax) {
      a10++;
      return true;
    }

    if (a10 == valmax && a9 < valmax) {
      a10 = 0;
      a9++;
      return true;
    }

    if (a10 == valmax && a9 == valmax && a8 < valmax) {
      a10 = 0;
      a9 = 0;
      a8++;
      return true;
    }

    if (a10 == valmax && a9 == valmax && a8 == valmax && a7 < valmax) {
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 < valmax) {
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 < valmax) {
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 < valmax) {
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 < valmax) {
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a10 == valmax && a9 == valmax && a8 == valmax && a7 == valmax &&
        a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax&&
        a2 == valmax && a1 < valmax) {
      a10 = 0;
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator9 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8, a9;
  int valmax;

public:
  Generator9() : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0), a9(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2], moves[a3], moves[a4], moves[a5],
                       moves[a6], moves[a7], moves[a8], moves[a9]};
  }

  bool operator++() {
    if (a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax &&  a3 == valmax && a2 == valmax &&
        a1 == valmax) {
      return false;
    }

    if (a9 < valmax ) {
      a9++;
      return true;
    }

    if (a9 == valmax && a8 < valmax) {
      a9 = 0;
      a8++;
      return true;
    }

    if (a9 == valmax && a8 == valmax && a7 < valmax) {
      a9 = 0;
      a8 = 0;
      a7++;
      return true;
    }

    if (a9 == valmax && a8 == valmax && a7 == valmax && a6 < valmax) {
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 < valmax) {
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 < valmax) {
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax &&  a3 < valmax) {
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 < valmax) {
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
      a3 = 0;
      a2++;
      return true;
    }

    if (a9 == valmax && a8 == valmax && a7 == valmax && a6 == valmax &&
        a5 == valmax && a4 == valmax && a3 == valmax&& a2 == valmax &&
        a1 < valmax) {
      a9 = 0;
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
      a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator8 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7, a8;
  int valmax;

public:
  Generator8() : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0), a8(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2], moves[a3], moves[a4],
                       moves[a5], moves[a6], moves[a7], moves[a8]};
  }

  bool operator++() {
    if (a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax &&
		a4 == valmax && a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a8 < valmax ) {
      a8++;
      return true;
    }

    if (a8 == valmax && a7 < valmax) {
      a8 = 0;
      a7++;
      return true;
    }

    if (a8 == valmax && a7 == valmax && a6 < valmax) {
      a8 = 0;
      a7 = 0;
      a6++;
      return true;
    }

    if (a8 == valmax && a7 == valmax && a6 == valmax && a5 < valmax) {
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 < valmax) {
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
         a3 < valmax) {
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
         a3 == valmax && a2 < valmax) {
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a8 == valmax && a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax && 
         a3 == valmax && a2 == valmax && a1 < valmax) {
      a8 = 0;
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator7 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6, a7;
  int valmax;

public:
  Generator7() : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0), a7(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2], moves[a3], moves[a4],
                       moves[a5], moves[a6], moves[a7]};
  }

  bool operator++() {
    if (a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
        a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a7 < valmax ) {
      a7++;
      return true;
    }

    if (a7 == valmax && a6 < valmax) {
      a7 = 0;
      a6++;
      return true;
    }

    if (a7 == valmax && a6 == valmax && a5 < valmax) {
      a7 = 0;
      a6 = 0;
      a5++;
      return true;
    }

    if (a7 == valmax && a6 == valmax && a5 == valmax && a4 < valmax) {
      a7 = 0;
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a7 == valmax && a6 == valmax && a5 == valmax && a4 == valmax &&
        a3 < valmax) {
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a7 == valmax && a6 == valmax && a5 == valmax &&  a4 == valmax &&
        a3 == valmax && a2 < valmax) {
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a7 == valmax && a6 == valmax && a5 == valmax &&  a4 == valmax &&
        a3 == valmax && a2 == valmax && a1 < valmax) {
      a7 = 0;
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator6 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5, a6;
  int valmax;

public:
  Generator6() : a1(0), a2(0), a3(0), a4(0), a5(0), a6(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2], moves[a3],
                       moves[a4], moves[a5], moves[a6]};
  }

  bool operator++() {
    if (a6 == valmax && a5 == valmax && a4 == valmax && a3 == valmax &&
        a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a6 < valmax ) {
      a6++;
      return true;
    }

    if (a6 == valmax && a5 < valmax) {
      a6 = 0;
      a5++;
      return true;
    }

    if (a6 == valmax && a5 == valmax && a4 < valmax) {
      a6 = 0;
      a5 = 0;
      a4++;
      return true;
    }

    if (a6 == valmax && a5 == valmax &&  a4 == valmax && a3 < valmax) {
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a6 == valmax && a5 == valmax &&  a4 == valmax && a3 == valmax &&
        a2 < valmax) {
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a6 == valmax && a5 == valmax &&  a4 == valmax && a3 == valmax &&
        a2 == valmax && a1 < valmax) {
      a6 = 0;
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator5 {
  std::vector<char> moves;
  int a1, a2, a3, a4, a5;
  int valmax;

public:
  Generator5() : a1(0), a2(0), a3(0), a4(0), a5(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2], moves[a3], moves[a4], moves[a5]};
  }

  bool operator++() {
    if (a5 == valmax && a4 == valmax && a3 == valmax && a2 == valmax &&
        a1 == valmax) {
      return false;
    }

    if (a5 < valmax ) {
      a5++;
      return true;
    }

    if (a5 == valmax && a4 < valmax) {
      a5 = 0;
      a4++;
      return true;
    }

    if (a5 == valmax &&  a4 == valmax && a3 < valmax) {
      a5 = 0;
	  a4 = 0;
            a3++;
      return true;
    }

    if (a5 == valmax &&   a4 == valmax && a3 == valmax && a2 < valmax) {
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2++;
      return true;
    }

    if (a5 == valmax &&  a4 == valmax && a3 == valmax && a2 == valmax &&
        a1 < valmax) {
      a5 = 0;
	  a4 = 0;
            a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator4 {
  std::vector<char> moves;
  int a1, a2, a3, a4;
  int valmax;

public:
  Generator4() : a1(0), a2(0), a3(0), a4(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2], moves[a3], moves[a4]};
  }

  bool operator++() {
    if ( a4 == valmax && a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if (a4 < valmax) {
      a4++;
      return true;
    }

    if ( a4 == valmax && a3 < valmax) {
		a4 = 0;
            a3++;
      return true;
    }

    if (a4 == valmax &&  a3 == valmax && a2 < valmax) {
	  a4 = 0;
      a3 = 0;
      a2++;
      return true;
    }

    if (a4 == valmax &&  a3 == valmax && a2 == valmax && a1 < valmax) {
	  a4 = 0;
      a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

class Generator3 {
  std::vector<char> moves;
  int a1, a2, a3;
  int valmax;

public:
  Generator3() : a1(0), a2(0), a3(0) {
    for (char i = char(32); i  <  char(126); i++)
      moves.emplace_back(i);
    valmax = moves.size() - 1;
  }

  std::string get() {
    return std::string{moves[a1], moves[a2], moves[a3]};
  }

  bool operator++() {
    if ( a3 == valmax && a2 == valmax && a1 == valmax) {
      return false;
    }

    if ( a3 < valmax) {
      a3++;
      return true;
    }

    if ( a3 == valmax && a2 < valmax) {
      a3 = 0;
      a2++;
      return true;
    }

    if ( a3 == valmax && a2 == valmax && a1 < valmax) {
      a3 = 0;
      a2 = 0;
      a1++;
      return true;
    }
  }
};

#endif