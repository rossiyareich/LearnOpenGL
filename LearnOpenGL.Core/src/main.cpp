#include <complex>

#include "IMain.h"
#include "pModelLoading.h"

int main()
{
    const std::unique_ptr<IMain> mainRunner{std::make_unique<pModelLoading>()};
    mainRunner->main();
}
