#pragma once

#define M_STR_HELPER(x) #x
#define M_STR(x) M_STR_HELPER(x)

#define M_PP std::string("Function: ")+__func__+"\nFile: "+__FILE__+"\nLine: "+std::to_string(__LINE__)+"\n"
#define M_ND [[nodiscard]]
#define M_DP [[deprecated]]
#define M_MU [[maybe_unused]]