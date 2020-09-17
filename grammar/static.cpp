#include <iostream>

class T{
    private:
        int a;
        int b;
        T(){
            a = 0;
            b = 0;
        }
    public:
        static T& instance(){
            static T t;
            return t;
        }
        ~T(){}
};

int main(){
    for(int i=0; i<10; i++){
        T t = T::instance();
        std::cout<<&t<<std::endl;
    }
    system("pause");
    return 0;
}