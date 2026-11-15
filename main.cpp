#include <iostream>
#include <chrono>
#include <string>

#include "./orderbook.cpp"


int main(){

    Orderbook book;

    book.submit_limit_order(100, 10, OrderSide::buy);
    book.submit_limit_order(105, 5, OrderSide::buy);


    int id;
    std::cout << " Enter ID to cancel : ";
    std::cin >> id;

    book.cancel_limit_order(id);
    book.fetch_order(id);

    return 0;
}
