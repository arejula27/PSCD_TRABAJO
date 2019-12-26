#include <iostream>
#include "caminante.hpp"
#include <gtest/gtest.h>

using namespace std;

TEST(CAMINANTE, codificar)
{
    Caminante cam;
    cam.ini(3, 6);
    string ruta;
    EXPECT_NO_FATAL_FAILURE(ruta = cam.codificar());
    cout << "cam empieza en 3 y pasa por 6: " << ruta << endl;
}

int main(int argc, char const *argv[])
{

    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}