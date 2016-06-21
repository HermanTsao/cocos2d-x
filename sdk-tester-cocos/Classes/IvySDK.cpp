#include "IvySDK.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
namespace IvySDK {
    onPaymentResult paymentCallback_;
    onFreecoinResult freeCoinCallback_;
    onSNSResult snsCallback_;
    onLeaderBoardResult leaderBoardCallback_;
}
#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT void JNICALL Java_com_risesdk_client_Cocos_pr(JNIEnv* env, jclass clazz, jint billingId, jboolean success) {
        CCLOG("payment result");
        if (IvySDK::paymentCallback_ != 0) {
            IvySDK::paymentCallback_(success ? IvySDK::PAYMENT_RESULT_SUCCESS : IvySDK::PAYMENT_RESULT_FAILURE, billingId);
        }
    }
    
    JNIEXPORT void JNICALL Java_com_risesdk_client_Cocos_pv(JNIEnv* env, jclass clazz) {
        CCLOG("payment system valid");
    }
    
    JNIEXPORT void JNICALL Java_com_risesdk_client_Cocos_rr(JNIEnv* env, jclass clazz, jint rewardId) {
        CCLOG("receive free coins");
        if (IvySDK::freeCoinCallback_ != 0)
            IvySDK::freeCoinCallback_(rewardId);
    }
    
    JNIEXPORT void JNICALL Java_com_risesdk_client_Cocos_sns(JNIEnv* env, jclass clazz, jint msg, jboolean success, jint extra){
        CCLOG("receive sns message: %d, result: %d", msg, success? 1 : 0);
        if (IvySDK::snsCallback_ != 0) {
            IvySDK::snsCallback_(msg, success, extra);
        }
    }
    
    JNIEXPORT void JNICALL Java_com_risesdk_client_Cocos_lb(JNIEnv* env, jclass clazz, jboolean submit, jboolean success, jstring leaderBoardId, jstring extra){
        CCLOG("receive leader board message: %s, result: %d", submit ? "submit" : "load", success? 1 : 0);
        if (IvySDK::leaderBoardCallback_ != 0) {
            const char* id = env->GetStringUTFChars(leaderBoardId, 0);
            env->DeleteLocalRef(leaderBoardId);
            const char* ex = env->GetStringUTFChars(extra, 0);
            env->DeleteLocalRef(extra);
            IvySDK::leaderBoardCallback_(submit, success, id, ex);
        }
    }
#ifdef __cplusplus
}
#endif
#endif
