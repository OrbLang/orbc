#include "log/log.hpp"
#include "log/status.hpp"

int main()
{
    size_t testingIdx = logging::LoadingBar::Insert("Lexing", 0.0, "Complete");
    size_t otherIdx = logging::LoadingBar::Insert("Parsing", 75.0, "Other crazy shit");

    logging::Error("I will now change \"Test\" to 40%\nhello");

    logging::LoadingBar::UpdateBar(testingIdx, 40.0, "eyo");

    // size_t fileIdx = logging::LoadingBar::Insert("Curr File", 0.0, "Compiling");
    // size_t TotalIdx = logging::LoadingBar::Insert("Total", 0.0, "Man this is a lot of files");
    //
    // for (int i = 1; i <= 10; i++)
    //{
    //     logging::LoadingBar::UpdateBar(fileIdx, static_cast<float>(i * 10),
    //                                    "Man this is a lot of files");
    //     for (int j = 1; j <= 10; j++)
    //     {
    //         logging::LoadingBar::UpdateBar(TotalIdx, static_cast<float>(j * 10),
    //                                        "Man this is a lot of files");
    //         std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //     }
    // }
}
