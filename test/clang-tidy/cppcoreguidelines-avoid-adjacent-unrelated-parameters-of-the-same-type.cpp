// RUN: %check_clang_tidy %s cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type %t

void twoParam(int, int);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'twoParam' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]

void threeParamXYY(bool, int&, int&);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'threeParamXYY' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]

void threeParamXXY(const int, const int, bool);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'threeParamXXY' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]

template<typename T>
void invalidTemplate(T a, T b);
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'invalidTemplate' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]

auto invalidLambda = [](bool, bool){return 12;};
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: function 'operator()' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]

auto invalidLambdaWithNestedDeclarations = [](bool, bool){
// CHECK-MESSAGES: :[[@LINE-1]]:44: warning: function 'operator()' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]
    struct A{
        void good(int);
        void bad(int, int);
        // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: function 'bad' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]
    };
    auto good = [](int, bool){return 10;};
    auto bad = [](int, int){return 10;};
    // TODO: this should generate warning: :[[@LINE-1]]:16: warning: function 'operator()' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]
    return good(10, 10) + bad(10, 10);
};

struct Q
{
    Q(int a, int b);
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: function 'Q' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]

    void twoParamMember(int* a, int* b);
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function 'twoParamMember' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]

    template<typename T>
    void twoParamMemberT(T a, T b);
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function 'twoParamMemberT' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]

    void operator()(float* a, float* b);
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function 'operator()' has two adjacent parameters of the same type [cppcoreguidelines-avoid-adjacent-unrelated-parameters-of-the-same-type]
};

bool operator==(const Q&, const Q&);
bool operator!=(const Q&, const Q&);
bool operator>=(const Q&, const Q&);
bool operator<=(const Q&, const Q&);
bool operator>(const Q&, const Q&);
bool operator<(const Q&, const Q&);
struct Comparer
{
    bool operator()(const Q& lhs, const Q& rhs) const;
};


void noParams();
void singleParam(int);

using IntAlias = int;
void twoParamAliased(IntAlias, int);
