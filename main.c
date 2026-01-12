#include <stdio.h>
#include <stdlib.h>
#include "bigbinary.h"

void printSeparator(const char *title) {
    printf("\n====================================\n");
    printf("        %s\n", title);
    printf("====================================\n");
}

void printMenu() {
    printf("\n===================================\n");
    printf("    CALCULATRICE BIGBINARY - MENU    \n");
    printf("=====================================\n");
    printf("| 1. Addition (A + B)               |\n");
    printf("| 2. Soustraction (A - B)           |\n");
    printf("| 3. Multiplication                 |\n");
    printf("| 4. Multiplication Egyptienne      |\n");
    printf("| 5. Division par 2                 |\n");
    printf("| 6. PGCD(A, B)                     |\n");
    printf("| 7. A mod B                        |\n");
    printf("| 8. Exponentiation modulaire       |\n");
    printf("| 9. Test RSA complet               |\n");
    printf("| 10. Tests automatiques            |\n");
    printf("| 0. Quitter                        |\n");
    printf("=====================================\n");
    printf("Votre choix : ");
}

void testsAutomatiques() {
    printSeparator("PHASE 1");

    // Initialisation
    BigBinary a = initBigBinaryDepuisChaine("1010111100001111000001111111111100000000011111111111111100000100000000000000100001110011001111111111000001000000000000001000011100110011111111100011110000010000000000000010000111001100111111111111100000111");
    BigBinary b = initBigBinaryDepuisChaine("0011111111111000110000111111111111110000000000000011111111111110000000000000000000111111");
    BigBinary z = initBigBinaryVide();

    printf("a = "); afficheBigBinary(a);
    printf("b = "); afficheBigBinary(b);
    printf("z = "); afficheBigBinary(z);

    // Addition
    BigBinary s = additionBigBinary(&a, &b);
    printf("a + b = ");
    afficheBigBinary(s);

    // Soustraction
    BigBinary d = soustractionBigBinary(&a, &b);
    printf("a - b = ");
    afficheBigBinary(d);

    // Division par 2
    BigBinary a_copy = copieBigBinary(&a);
    printf("a / 2 = ");
    divisePar2(&a_copy);
    afficheBigBinary(a_copy);

    // Comparaisons
    printf("a == b ? %d\n", Egal(&a, &b));
    printf("a <  b ? %d\n", Inferieur(&a, &b));
    printf("b <  s ? %d\n", Inferieur(&b, &s));
    printf("s == s ? %d\n", Egal(&s, &s));

    libereBigBinary(&a);
    libereBigBinary(&b);
    libereBigBinary(&z);
    libereBigBinary(&s);
    libereBigBinary(&d);
    libereBigBinary(&a_copy);

    // PHASE 2 : PGCD
    printSeparator("PHASE 2 : PGCD");

    BigBinary A = initBigBinaryDepuisChaine("101001111111111100101001001111111111100000000000111111111");
    BigBinary B = initBigBinaryDepuisChaine("111001");

    printf("A = "); afficheBigBinary(A);
    printf("B = "); afficheBigBinary(B);

    BigBinary G = BigBinary_PGCD(A, B);
    printf("PGCD(A,B) = ");
    afficheBigBinary(G);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&G);

    // MULTIPLICATION
    printSeparator("MULTIPLICATION");

    BigBinary m1 = initBigBinaryDepuisChaine("1011"); // 11
    BigBinary m2 = initBigBinaryDepuisChaine("110");  // 6

    printf("m1 = "); afficheBigBinary(m1);
    printf("m2 = "); afficheBigBinary(m2);

    BigBinary M_mult = BigBinary_multiply(&m1, &m2);
    printf("m1 * m2 = ");
    afficheBigBinary(M_mult);
    printf("(Attendu : 1000010 = 66 en decimal)\n");

    libereBigBinary(&m1);
    libereBigBinary(&m2);
    libereBigBinary(&M_mult);

    // SOUSTRACTION
    printSeparator("SOUSTRACTION");

    BigBinary X = initBigBinaryDepuisChaine("10101"); // 21
    BigBinary Y = initBigBinaryDepuisChaine("00110"); // 6
    BigBinary Rsub = soustractionBigBinary(&X, &Y);

    printf("21 - 6 = ");
    afficheBigBinary(Rsub);
    printf("(Attendu : 1111 = 15 en decimal)\n");

    libereBigBinary(&X);
    libereBigBinary(&Y);
    libereBigBinary(&Rsub);

    // MODULO
    printSeparator("MODULO");

    BigBinary mo1 = initBigBinaryDepuisChaine("10101"); // 21
    BigBinary mo2 = initBigBinaryDepuisChaine("00110"); // 6

    BigBinary R1 = BigBinary_mod(mo1, mo2);
    printf("21 mod 6 = ");
    afficheBigBinary(R1);
    printf("(Attendu : 11 = 3 en decimal)\n");

    libereBigBinary(&mo1);
    libereBigBinary(&mo2);
    libereBigBinary(&R1);

    // MULTIPLICATION EGYPTIENNE
    printSeparator("MULTIPLICATION EGYPTIENNE");

    BigBinary e1 = initBigBinaryDepuisChaine("1101"); // 13
    BigBinary e2 = initBigBinaryDepuisChaine("1011"); // 11

    BigBinary R_egy = BigBinary_multiplyEgypt(&e1, &e2);
    printf("13 * 11 = ");
    afficheBigBinary(R_egy);
    printf("(Attendu : 10001111 = 143 en decimal)\n");

    libereBigBinary(&e1);
    libereBigBinary(&e2);
    libereBigBinary(&R_egy);

    // EXPONENTIATION MODULAIRE
    printSeparator("EXPONENTIATION MODULAIRE");

    BigBinary base = initBigBinaryDepuisChaine("101");    // 5
    BigBinary mod  = initBigBinaryDepuisChaine("1101");   // 13

    BigBinary res = BigBinary_expMod(base, 3, mod);
    printf("5^3 mod 13 = ");
    afficheBigBinary(res);
    printf("(Attendu : 1000 = 8 en decimal)\n");

    libereBigBinary(&base);
    libereBigBinary(&mod);
    libereBigBinary(&res);
    
    // PHASE 3 : TEST RSA
    printSeparator("PHASE 3 : RSA COMPLET");
    
    BigBinary M = intToBigBinary(99999);
    BigBinary E = intToBigBinary(101);
    BigBinary N = intToBigBinary(1211809);
    BigBinary D = intToBigBinary(251501);
    
    printf("Message original M = ");
    afficheBigBinary(M);
    printf("En decimal : %u\n\n", bigBinaryToInt(M));
    
    printf("Cle publique (N, E) :\n");
    printf("  N = ");
    afficheBigBinary(N);
    printf("  (decimal : %u)\n", bigBinaryToInt(N));
    printf("  E = ");
    afficheBigBinary(E);
    printf("  (decimal : %u)\n\n", bigBinaryToInt(E));
    
    printf("--- CHIFFREMENT ---\n");
    BigBinary C = BigBinary_RSA_Encrypt(M, E, N);
    printf("\nMessage chiffre C = ");
    afficheBigBinary(C);
    printf("En decimal : %u\n", bigBinaryToInt(C));
    printf("(Attendu : 561752)\n");
    
    printf("\n--- DECHIFFREMENT ---\n");
    BigBinary M_dechiffre = BigBinary_RSA_Decrypt(C, D, N);
    printf("\nMessage dechiffre = ");
    afficheBigBinary(M_dechiffre);
    printf("En decimal : %u\n", bigBinaryToInt(M_dechiffre));
    printf("(Devrait etre : 99999)\n");
    
    if (Egal(&M, &M_dechiffre)) {
        printf("\n✓ SUCCESS : Le dechiffrement a reussi !\n");
    } else {
        printf("\n✗ ERREUR : Le dechiffrement a echoue.\n");
    }
    
    libereBigBinary(&M);
    libereBigBinary(&E);
    libereBigBinary(&N);
    libereBigBinary(&D);
    libereBigBinary(&C);
    libereBigBinary(&M_dechiffre);
}

int main(void) {
    char buffer[10000];
    int choix;
    
    printf("=====================================\n");
    printf("|         PROJET BIGBINARY          |\n");
    printf("|       Manele - Pavel -Ines        |\n");
    printf("=====================================\n");
    
    do {
        printMenu();
        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            choix = -1;
            continue;
        }
        
        if (choix == 0) break;
        
        BigBinary a, b, result;
        
        switch(choix) {
            case 1: // Addition
                printSeparator("ADDITION");
                printf("Entrez A (binaire) : ");
                scanf("%s", buffer);
                a = initBigBinaryDepuisChaine(buffer);
                
                printf("Entrez B (binaire) : ");
                scanf("%s", buffer);
                b = initBigBinaryDepuisChaine(buffer);
                
                printf("\nA = "); afficheBigBinary(a);
                printf("B = "); afficheBigBinary(b);
                
                result = additionBigBinary(&a, &b);
                printf("\nA + B = "); afficheBigBinary(result);
                
                libereBigBinary(&a);
                libereBigBinary(&b);
                libereBigBinary(&result);
                break;
                
            case 2: // Soustraction
                printSeparator("SOUSTRACTION");
                printf("Entrez A (binaire) : ");
                scanf("%s", buffer);
                a = initBigBinaryDepuisChaine(buffer);
                
                printf("Entrez B (binaire) : ");
                scanf("%s", buffer);
                b = initBigBinaryDepuisChaine(buffer);
                
                printf("\nA = "); afficheBigBinary(a);
                printf("B = "); afficheBigBinary(b);
                
                if (!Inferieur(&a, &b)) {
                    result = soustractionBigBinary(&a, &b);
                    printf("\nA - B = "); afficheBigBinary(result);
                    libereBigBinary(&result);
                } else {
                    printf("\nERREUR : A < B, soustraction impossible\n");
                }
                
                libereBigBinary(&a);
                libereBigBinary(&b);
                break;
                
            case 3: // Multiplication
                printSeparator("MULTIPLICATION");
                printf("Entrez A (binaire) : ");
                scanf("%s", buffer);
                a = initBigBinaryDepuisChaine(buffer);
                
                printf("Entrez B (binaire) : ");
                scanf("%s", buffer);
                b = initBigBinaryDepuisChaine(buffer);
                
                printf("\nA = "); afficheBigBinary(a);
                printf("B = "); afficheBigBinary(b);
                
                result = BigBinary_multiply(&a, &b);
                printf("\nA × B = "); afficheBigBinary(result);
                
                libereBigBinary(&a);
                libereBigBinary(&b);
                libereBigBinary(&result);
                break;
                
            case 4: // Multiplication Egyptienne
                printSeparator("MULTIPLICATION EGYPTIENNE");
                printf("Entrez A (binaire) : ");
                scanf("%s", buffer);
                a = initBigBinaryDepuisChaine(buffer);
                
                printf("Entrez B (binaire) : ");
                scanf("%s", buffer);
                b = initBigBinaryDepuisChaine(buffer);
                
                printf("\nA = "); afficheBigBinary(a);
                printf("B = "); afficheBigBinary(b);
                
                result = BigBinary_multiplyEgypt(&a, &b);
                printf("\nA × B (Egyptien) = "); afficheBigBinary(result);
                
                libereBigBinary(&a);
                libereBigBinary(&b);
                libereBigBinary(&result);
                break;
                
            case 5: // Division par 2
                printSeparator("DIVISION PAR 2");
                printf("Entrez A (binaire) : ");
                scanf("%s", buffer);
                a = initBigBinaryDepuisChaine(buffer);
                
                printf("\nA = "); afficheBigBinary(a);
                divisePar2(&a);
                printf("A / 2 = "); afficheBigBinary(a);
                
                libereBigBinary(&a);
                break;
                
            case 6: // PGCD
                printSeparator("PGCD");
                printf("Entrez A (binaire) : ");
                scanf("%s", buffer);
                a = initBigBinaryDepuisChaine(buffer);
                
                printf("Entrez B (binaire) : ");
                scanf("%s", buffer);
                b = initBigBinaryDepuisChaine(buffer);
                
                printf("\nA = "); afficheBigBinary(a);
                printf("B = "); afficheBigBinary(b);
                
                result = BigBinary_PGCD(a, b);
                printf("\nPGCD(A, B) = "); afficheBigBinary(result);
                
                libereBigBinary(&a);
                libereBigBinary(&b);
                libereBigBinary(&result);
                break;
                
            case 7: // Modulo
                printSeparator("MODULO");
                printf("Entrez A (binaire) : ");
                scanf("%s", buffer);
                a = initBigBinaryDepuisChaine(buffer);
                
                printf("Entrez B (binaire) : ");
                scanf("%s", buffer);
                b = initBigBinaryDepuisChaine(buffer);
                
                printf("\nA = "); afficheBigBinary(a);
                printf("B = "); afficheBigBinary(b);
                
                if (b.Signe != 0) {
                    result = BigBinary_mod(a, b);
                    printf("\nA mod B = "); afficheBigBinary(result);
                    libereBigBinary(&result);
                } else {
                    printf("\nERREUR : Division par zero\n");
                }
                
                libereBigBinary(&a);
                libereBigBinary(&b);
                break;
                
            case 8: // Exponentiation modulaire
                printSeparator("EXPONENTIATION MODULAIRE");
                printf("Entrez M (base, binaire) : ");
                scanf("%s", buffer);
                a = initBigBinaryDepuisChaine(buffer);
                
                int exp;
                printf("Entrez l'exposant (decimal) : ");
                scanf("%d", &exp);
                
                printf("Entrez N (modulo, binaire) : ");
                scanf("%s", buffer);
                b = initBigBinaryDepuisChaine(buffer);
                
                printf("\nM = "); afficheBigBinary(a);
                printf("Exposant = %d\n", exp);
                printf("N = "); afficheBigBinary(b);
                
                result = BigBinary_expMod(a, exp, b);
                printf("\nM^%d mod N = ", exp); afficheBigBinary(result);
                
                libereBigBinary(&a);
                libereBigBinary(&b);
                libereBigBinary(&result);
                break;
                
            case 9: // RSA
                printSeparator("TEST RSA COMPLET");
                printf("\n--- PARAMETRES RSA (en decimal) ---\n");
                
                unsigned int M_val, E_val, N_val, D_val;
                
                printf("Message M : ");
                scanf("%u", &M_val);
                
                printf("Exposant public E : ");
                scanf("%u", &E_val);
                
                printf("Module N : ");
                scanf("%u", &N_val);
                
                printf("Exposant prive D : ");
                scanf("%u", &D_val);
                
                BigBinary M = intToBigBinary(M_val);
                BigBinary E = intToBigBinary(E_val);
                BigBinary N = intToBigBinary(N_val);
                BigBinary D = intToBigBinary(D_val);
                
                printf("\n--- VALEURS EN BINAIRE ---\n");
                printf("M = "); afficheBigBinary(M);
                printf("E = "); afficheBigBinary(E);
                printf("N = "); afficheBigBinary(N);
                printf("D = "); afficheBigBinary(D);
                
                printf("\n--- CHIFFREMENT ---\n");
                BigBinary C = BigBinary_RSA_Encrypt(M, E, N);
                printf("Message chiffre C = ");
                afficheBigBinary(C);
                printf("En decimal : %u\n", bigBinaryToInt(C));
                
                printf("\n--- DECHIFFREMENT ---\n");
                BigBinary M2 = BigBinary_RSA_Decrypt(C, D, N);
                printf("Message dechiffre = ");
                afficheBigBinary(M2);
                printf("En decimal : %u\n", bigBinaryToInt(M2));
                
                if (Egal(&M, &M2)) {
                    printf("\n✓ RSA fonctionne correctement !\n");
                } else {
                    printf("\n✗ Erreur dans le processus RSA\n");
                }
                
                libereBigBinary(&M);
                libereBigBinary(&E);
                libereBigBinary(&N);
                libereBigBinary(&D);
                libereBigBinary(&C);
                libereBigBinary(&M2);
                break;
                
            case 10: // Tests automatiques
                testsAutomatiques();
                break;
                
            default:
                printf("\n✗ Choix invalide. Reessayez.\n");
        }
        
        printf("\nAppuyez sur Entree pour continuer...");
        while(getchar() != '\n');
        getchar();
        
    } while (choix != 0);
    
    printf("\n==================================\n");
    printf("|          FIN DU PROJET            |\n");
    printf("|            BIGBINARY              |\n");
    printf("=====================================\n\n");
    
    return 0;
}