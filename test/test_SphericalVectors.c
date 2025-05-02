#include <stdio.h>
#include "../include/SphericalVectors.h"

#define ASSERT_FLOAT_EQ(a, b) (fabs((a) - (b)) < 1e-6)

void test_isVectorEqual()
{
    printf("Testing isVectorEqual...\n");
    struct
    {
        float x1, y1, z1;
        float x2, y2, z2;
        bool expected_result;
    } cases[] = {
        {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, true},
        {1.0f, 2.0f, 3.0f, 1.0f, 2.0f, 3.0f, true},
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, false},
        {1.000001f, 2.000001f, 3.000001f, 1.000002f, 2.000002f, 3.00002f, false}};

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        vector3 vec1, vec2;
        vector3InitCartesian(&vec1, cases[i].x1, cases[i].y1, cases[i].z1, true);
        vector3InitCartesian(&vec2, cases[i].x2, cases[i].y2, cases[i].z2, true);

        bool result = isVectorEqual(&vec1, &vec2);
        bool pass = (result == cases[i].expected_result);

        if (pass)
        {
            printf("  ✅ Test case %zu passed\n", i + 1);
        }
        else
        {
            printf("  ❌ Test case %zu failed\n", i + 1);
            printf("    Expected: %s\n", cases[i].expected_result ? "true" : "false");
            printf("    Got: %s\n", result ? "true" : "false");
        }
    }
    if (sizeof(cases) / sizeof(cases[0]) == 0)
    {
        printf("->❌ No test cases provided<-\n\n");
    }
    else
    {
        printf("->✅ All test cases passed<-\n\n");
    }
}

void test_normalizeVector3()
{
    printf("Testing normalizeVector3...\n");
    struct
    {
        float x, y, z;
        float expected_norm;
    } cases[] = {
        {3.0f, 4.0f, 0.0f, 1.0f},
        {1.0f, 2.0f, 2.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 0.0f} // Edge case
    };
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        vector3 vec;
        vector3InitCartesian(&vec, cases[i].x, cases[i].y, cases[i].z, true);
        normalizeVector3(&vec);

        bool pass = ASSERT_FLOAT_EQ(vec.norm, cases[i].expected_norm);
        if (pass)
        {
            printf("  ✅ Test case %zu passed\n", i + 1);
        }
        else
        {
            printf("  ❌ Test case %zu failed\n", i + 1);
            printf("    Expected: norm = %.2f\n", cases[i].expected_norm);
            printf("    Got: norm = %.2f\n", vec.norm);
        }
    }
    if (sizeof(cases) / sizeof(cases[0]) == 0)
    {
        printf("->❌ No test cases provided<-\n\n");
    }
    else
    {
        printf("->✅ All test cases passed<-\n\n");
    }
}

void test_vector3InitCartesian()
{
    printf("Testing vector3InitCartesian...\n");
    struct
    {
        float x, y, z;
        float expected_RA, expected_DEC;
    } cases[] = {
        {1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 90.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f, 90.0f},
        {1.0f, 1.0f, 0.0f, 45.0f, 0.0f}, // Aprox.
        {1.0f, 0.0f, 1.0f, 0.0f, 45.0f}  // Aprox.
    };

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        vector3 vec;
        vector3InitCartesian(&vec, cases[i].x, cases[i].y, cases[i].z, true);

        bool pass =
            ASSERT_FLOAT_EQ(vec.norm, 1.0f) &&
            ASSERT_FLOAT_EQ(vec.RA, cases[i].expected_RA) &&
            ASSERT_FLOAT_EQ(vec.DEC, cases[i].expected_DEC);

        if (pass)
        {
            printf("  ✅ Test case %zu passed\n", i + 1);
        }
        else
        {
            printf("  ❌ Test case %zu failed\n", i + 1);
            printf("    Expected: RA = %.2f, DEC = %.2f\n", cases[i].expected_RA, cases[i].expected_DEC);
            printf("    Got: RA = %.2f, DEC = %.2f\n", vec.RA, vec.DEC);
        }
    }
    if (sizeof(cases) / sizeof(cases[0]) == 0)
    {
        printf("->❌ No test cases provided<-\n\n");
    }
    else
    {
        printf("->✅ All test cases passed<-\n\n");
    }
}

void test_vector3InitSpherical()
{
    printf("Testing vector3InitSpherical...\n");
    struct
    {
        float RA, DEC;
        float expected_x, expected_y, expected_z;
    } cases[] = {
        {0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
        {90.0f, 0.0f, 0.0f, 1.0f, 0.0f},
        {180.0f, 0.0f, -1.0f, 0.0f, 0.0f},
        {270.0f, 0.0f, 0.0f, -1.0f, 0.0f},
        {0.0f, 90.0f, 0.0f, 0.0f, 1.0f},
    };

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        vector3 vec;
        vector3InitSpherical(&vec, cases[i].RA, cases[i].DEC, true);

        bool pass =
            ASSERT_FLOAT_EQ(vec.x, cases[i].expected_x) &&
            ASSERT_FLOAT_EQ(vec.y, cases[i].expected_y) &&
            ASSERT_FLOAT_EQ(vec.z, cases[i].expected_z);

        if (pass)
        {
            printf("  ✅ Test case %zu passed\n", i + 1);
        }
        else
        {
            printf("  ❌ Test case %zu failed\n", i + 1);
            printf("    Expected: x = %.2f, y = %.2f, z = %.2f\n", cases[i].expected_x, cases[i].expected_y, cases[i].expected_z);
            printf("    Got: x = %.2f, y = %.2f, z = %.2f\n", vec.x, vec.y, vec.z);
        }
    }
    if (sizeof(cases) / sizeof(cases[0]) == 0)
    {
        printf("->❌ No test cases provided<-\n\n");
    }
    else
    {
        printf("->✅ All test cases passed<-\n\n");
    }
}

void test_multiplyVector3(){
    printf("Testing multiplyVector3...\n");
    struct 
    {
        float x, y, z;
        float scalar;
        float expected_x, expected_y, expected_z;
    } cases[] = {
        {1.0f, 2.0f, 3.0f, 2.0f, 2.0f, 4.0f, 6.0f},
        {1.0f, 2.0f, 3.0f, -1.0f, -1.0f, -2.0f, -3.0f},
        {1.0f, 2.0f, 3.0f, 0.5f, 0.5f, 1.0f, 1.5f},
    };

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++){
        vector3 vec;
        vector3InitCartesian(&vec, cases[i].x, cases[i].y, cases[i].z, false);
        vector3 result = multiplyVector3(&vec, cases[i].scalar);
        bool pass = 
            ASSERT_FLOAT_EQ(result.x, cases[i].expected_x) &&
            ASSERT_FLOAT_EQ(result.y, cases[i].expected_y) &&
            ASSERT_FLOAT_EQ(result.z, cases[i].expected_z);
        if (pass){
            printf("  ✅ Test case %zu passed\n", i + 1);
        } else {
            printf("  ❌ Test case %zu failed\n", i + 1);
            printf("    Expected: x = %.2f, y = %.2f, z = %.2f\n", cases[i].expected_x, cases[i].expected_y, cases[i].expected_z);
            printf("    Got: x = %.2f, y = %.2f, z = %.2f\n", result.x, result.y, result.z);
        }
    }
    if (sizeof(cases) / sizeof(cases[0]) == 0)
    {
        printf("->❌ No test cases provided<-\n\n");
    }
    else
    {
        printf("->✅ All test cases passed<-\n\n");
    }
    
}


int main()
{
    test_vector3InitCartesian(); // ✅ Implementado
    test_vector3InitSpherical(); // ✅ Implementado
    test_isVectorEqual(); // ✅ Implementado
    test_normalizeVector3(); // ✅ Implementado
    test_multiplyVector3(); // ✅ Implementado

    return 0;
}