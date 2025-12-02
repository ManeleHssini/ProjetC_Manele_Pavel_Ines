#include "rsa.h"
#include <stdio.h>

// φ(n) = (p-1)(q-1)
BigBinary RSA_phi(BigBinary p, BigBinary q) {

    BigBinary un = initBigBinaryDepuisChaine("1");

    BigBinary p1 = soustractionBigBinary(&p, &un);
    BigBinary q1 = soustractionBigBinary(&q, &un);

    BigBinary phi = BigBinary_multiplyEgypt(&p1, &q1);

    libereBigBinary(&p1);
    libereBigBinary(&q1);
    libereBigBinary(&un);

    return phi;
}

// Algorithme d’Euclide Étendu pour trouver l’inverse modulaire
// Retourne a^-1 mod mod
BigBinary RSA_inverseMod(BigBinary a, BigBinary mod) {

    BigBinary zero = initBigBinaryVide();
    BigBinary un   = initBigBinaryDepuisChaine("1");

    BigBinary r0 = mod;
    BigBinary r1 = a;

    BigBinary t0 = initBigBinaryVide();     // 0
    BigBinary t1 = initBigBinaryDepuisChaine("1");  // 1

    while (r1.Signe != 0) {

        // q = r0 / r1  (division naive via soustractions)
        BigBinary q = initBigBinaryVide();
        BigBinary tempR0 = copieBigBinary(&r0);

        while (!Inferieur(&tempR0, &r1)) {
            BigBinary oneMore = additionBigBinary(&q, &un);
            libereBigBinary(&q);
            q = oneMore;

            BigBinary tmpSub = soustractionBigBinary(&tempR0, &r1);
            libereBigBinary(&tempR0);
            tempR0 = tmpSub;
        }

        libereBigBinary(&tempR0);

        // (r0, r1) = (r1, r0 - q*r1)
        BigBinary qr1 = BigBinary_multiplyEgypt(&q, &r1);
        BigBinary newr = soustractionBigBinary(&r0, &qr1);

        libereBigBinary(&r0);
        r0 = r1;

        libereBigBinary(&r1);
        r1 = newr;

        libereBigBinary(&qr1);

        // (t0, t1) = (t1, t0 - q*t1)
        BigBinary qt1 = BigBinary_multiplyEgypt(&q, &t1);
        BigBinary newt = soustractionBigBinary(&t0, &qt1);

        libereBigBinary(&t0);
        t0 = t1;

        libereBigBinary(&t1);
        t1 = newt;

        libereBigBinary(&qt1);
        libereBigBinary(&q);
    }

    // t0 = t0 mod mod
    BigBinary result = BigBinary_mod(t0, mod);

    libereBigBinary(&t0);
    libereBigBinary(&t1);
    libereBigBinary(&r0);
    libereBigBinary(&r1);
    libereBigBinary(&zero);
    libereBigBinary(&un);

    return result;
}

RSAKey RSA_generateKey(BigBinary p, BigBinary q, BigBinary e) {

    RSAKey key;

    key.n = BigBinary_multiplyEgypt(&p, &q);

    BigBinary phi = RSA_phi(p, q);

    key.e = copieBigBinary(&e);

    key.d = RSA_inverseMod(e, phi);

    libereBigBinary(&phi);

    return key;
}


// Convertir un BigBinary en int (exposant < 64 bits)
int BigBinary_toInt(BigBinary b) {
    int r = 0;
    for (int i = 0; i < b.Taille; i++) {
        r = (r << 1) + b.Tdigits[i];
    }
    return r;
}

BigBinary RSA_chiffre(BigBinary message, RSAKey key) {
    int e_int = BigBinary_toInt(key.e);
    return BigBinary_expMod(message, e_int, key.n);
}

BigBinary RSA_dechiffre(BigBinary message, RSAKey key) {
    int d_int = BigBinary_toInt(key.d);
    return BigBinary_expMod(message, d_int, key.n);
}
