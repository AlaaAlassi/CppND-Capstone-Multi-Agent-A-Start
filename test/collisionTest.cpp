#include <gtest/gtest.h>


TEST(CollisionTest,BasicTest){

ASSERT_EQ(5*5,25);

}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}