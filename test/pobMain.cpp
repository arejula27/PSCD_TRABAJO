
//compile: make pobtest -f Makefile_p5_e1
#include <iostream>
#include "caminante.hpp"
#include <gtest/gtest.h>
using namespace std;

/*****
 * Prueba                    Demuestra
 * EXPECT_EQ(int val1, int val2);	val1 == val2
 * EXPECT_NE(int val1, int val2);   val1 != val2
 * EXPECT_LT(int val1, int val2);	val1 < val2
 * EXPECT_LE(int val1, int val2);	val1 <= val2
 * EXPECT_GT(int val1, int val2);	val1 > val2
 * EXPECT_GE(int val1, int val2);	val1 >= val2
 * 
 * EXPECT_TRUE(bool val1 )          true
 * EXPECT_FALSE(bool val1)          false
 * 
 * EXPECT_NO_FATAL_FAILURE() lo ejecuta y mira que finaliza 
 * 
 * EXPECT_STREQ(str1,str2);	dos strings son iguales
 * EXPECT_STRNE(str1,str2);	dos strings son distintos
 * EXPECT_STRCASEEQ(str1,str2);	dos strings son iguales, da igual mayusculas y minusculas
 * EXPECT_STRCASENE(str1,str2);	dos strings son distintos, da igual mayusculas y minusculas
 * 
 *****/







//Creo un test con titulo,subtitulo
string ruta,ruta2;
TEST(CAMINANTE, codificar)
{
    Caminante cam;
    cam.ini(3, 6);
    string ruta;
    EXPECT_NO_FATAL_FAILURE(ruta = cam.codificar());
    cout << "cam empieza en 3 y pasa por 6: " << ruta << endl;
    
}
TEST(CAMINANTE, descodificar)
{
   
    Caminante cam2;
    int n;
    EXPECT_NO_FATAL_FAILURE(cam2.desCodificar(ruta, n));
    EXPECT_NO_FATAL_FAILURE(ruta2 = cam2.codificar());
    cout << "cam2: " << cam2.codificar() << endl;
    EXPECT_EQ(ruta, ruta2);
}

TEST(CAMINANTE, descodificar)
{

    Caminante cam2;
    int n;
    EXPECT_NO_FATAL_FAILURE(cam2.desCodificar(ruta, n));
    EXPECT_NO_FATAL_FAILURE(ruta2 = cam2.codificar());
    cout << "cam2: " << cam2.codificar() << endl;
    EXPECT_EQ(ruta, ruta2);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}