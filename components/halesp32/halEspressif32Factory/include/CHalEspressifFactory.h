#include "commonStd.h"

#include "IAdc.h"
#include "IBme280.h"
#include "IWifi.h"
#include "IHttpsClient.h"

class CHalEspressifFactory {
public:
    static IAdc& createAdc(void);
    static void destroyAdc(void);

    static IBme280& createBme280(void);
    static void destroyBme280(void);

    static IWifi& createWifi(void);
    static void destroyWifi(void);

    // static IHttpsClient& createHttpsClient(void);
    // static void destroyHttpsClient(void);
};