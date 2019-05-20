#include <iostream>
#include <functional>

std::function<void()> ww()
{
    int x=1;
    return [x]() mutable {
        printf("%d",x);
        x--;
        return;
    };
}

int main() {
    auto a = ww(), b = ww();
    a();
    b();
    a();
    b();

}
