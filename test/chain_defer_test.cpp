#include "promise.hpp"
#include <sstream>
#include <iostream>

promise::Defer writeTo(std::ostream& out) {
    return promise::newHeadPromise().then([&out](int value) {
        out << value;
        return promise::reject(std::string(" testErrorReason "));
    }, [&out](const std::string& reason) {
        out << reason;
        return 456;
    });
}

int main() {
    promise::Defer d = promise::newHeadPromise();
    std::ostringstream out;
    promise::Defer writeToOut = writeTo(out);
    d.then(writeToOut).then(writeTo(out)).then(writeTo(out));
    d.resolve(123);

    std::string expected = "123 testErrorReason 456";
    if (out.str() != expected) {
        std::cout << "FAIL chain_defer_test got \"" << out.str() << "\", "
                  << "expected \"" << expected << "\"\n";
        return 1;
    }
    std::cout << "PASS\n";
}
