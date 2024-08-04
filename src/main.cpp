#include "log/log.hpp"
#include "log/status.hpp"

int main()
{
    logging::LoadingBar lexerBar{"Lexing", 0.0f, "Reading main.orb", false};
    logging::LoadingBar parseBar{"Parsing", 0.0f, "Parsing to AST", false};

    logging::Error("I will now change \"Test\" to 40%");


    lexerBar.UpdateBar(40.0f, "eyo");

    logging::LoadingBar fileBar{"Curr File", 0.0, "Compiling", false};
    logging::LoadingBar totalBar{"Total", 0.0, "Man this is a lot of files", false};

    for (int i = 1; i <= 10; i++)
    {
        totalBar.UpdateBar(static_cast<float>(i * 10), "Man this is a lot of files");
        for (int j = 1; j <= 20; j++)
        {
            fileBar.UpdatePercent(static_cast<float>(j * 5));
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }
}
