#include <lib/AbelianSandpileModel.h>
#include <cstring>
#include <fstream>


/*   Constants   */

const char kInput[] = "-i";
const uint8_t kInputLen = 2;
const char kInputLong[] = "--input=";
const uint8_t kInputLongLen = 8;

const char kOutput[] = "-o";
const uint8_t kOutputLen = 2;
const char kOutputLong[] = "--output=";
const uint8_t kOutputLongLen = 9;

const char kMaxIter[] = "-m";
const uint8_t kMaxIterLen = 2;
const char kMaxIterLong[] = "--max-iter=";
const uint8_t kMaxIterLongLen = 11;

const char kFreq[] = "-f";
const uint8_t kFreqLen = 2;
const char kFreqLong[] = "--freq=";
const uint8_t kFreqLongLen = 7;

const uint8_t kInputArgSize = 255;
const uint8_t kOutputArgSize = 255;

const int16_t kMaxOfInt16T = 32767;
const int16_t kMinOfInt16T = -32768;


enum ErrorStatus {
    kNoArgsEntered,
    kUnknownArg,
    kMoreThanOneInputArg,
    kMoreThanOneOutputArg,
    kMoreThanOneMaxIterArg,
    kMoreThanOneFreqArg,
    kNotAValidInputArg,
    kNotAValidOutputArg,
    kNotAValidMaxIterArg,
    kNotAValidFreqArg,
};


struct Arguments {
    /* variables for the values of possible arguments */
    char input[kInputArgSize]{};
    char output[kOutputArgSize]{};
    uint64_t max_iter = 0;
    uint64_t freq = 0;
};


/*   logging   */

void ErrorOutput(ErrorStatus error_status) {
    /* displays all errors */
    switch (error_status) {
        case ErrorStatus::kNoArgsEntered:
            std::cerr << "You didn't pass any program arguments!\n";
            break;
        case ErrorStatus::kUnknownArg:
            std::cerr << "You entered an unknown argument!\n";
            break;
        case ErrorStatus::kMoreThanOneInputArg:
            std::cerr << "You entered more than one input argument!\n";
            break;
        case ErrorStatus::kMoreThanOneOutputArg:
            std::cerr << "You entered more than one output argument!\n";
            break;
        case ErrorStatus::kMoreThanOneMaxIterArg:
            std::cerr << "You entered more than one max iter argument!\n";
            break;
        case ErrorStatus::kMoreThanOneFreqArg:
            std::cerr << "You entered more than one freq argument!\n";
            break;
        case ErrorStatus::kNotAValidInputArg:
            std::cerr << "You didn't enter -i or --input or the correct value was not followed!\n";
            break;
        case ErrorStatus::kNotAValidOutputArg:
            std::cerr << "You didn't enter -o or --output or the correct value was not followed!\n";
            break;
        case ErrorStatus::kNotAValidMaxIterArg:
            std::cerr << "You didn't enter -m or --max-iter or the correct value was not followed!\n";
            break;
        case ErrorStatus::kNotAValidFreqArg:
            std::cerr << "You entered -f or --freq, but the correct value was not followed!\n";
            break;
    }
    std::exit(EXIT_FAILURE);
}


/*   checkers   */

bool IsItInputArg(const char* arg) {
    return (strncmp(arg, kInput, kInputLen) == 0);
}

bool IsItLongInputArg(const char* arg) {
    return (strncmp(arg, kInputLong, kInputLongLen) == 0);
}

bool IsItOutputArg(const char* arg) {
    return (strncmp(arg, kOutput, kOutputLen) == 0);
}

bool IsItLongOutputArg(const char* arg) {
    return (strncmp(arg, kOutputLong, kOutputLongLen) == 0);
}

bool IsItMaxIterArg(const char* arg) {
    return (strncmp(arg, kMaxIter, kMaxIterLen) == 0);
}

bool IsItLongMaxIterArg(const char* arg) {
    return (strncmp(arg, kMaxIterLong, kMaxIterLongLen) == 0);
}

bool IsItFreqArg(const char* arg) {
    return (strncmp(arg, kFreq, kFreqLen) == 0);
}

bool IsItLongFreqArg(const char* arg) {
    return (strncmp(arg, kFreqLong, kFreqLongLen) == 0);
}

bool IsNumber(const char* arg) {
    /* checks whether a string "arg" is a number */
    while (*arg != '\0') {
        if (!isdigit(*arg)) {
            return false;
        }
        ++arg;
    }

    return true;
}


/*   secondary functions   */

uint8_t GetIntLen(uint64_t num) {
    uint8_t count = 0;
    while (num != 0) {
        num /= 10;
        count += 1;
    }
    return count;
}

char* IntToStr(uint64_t num) {
    uint8_t len_int = GetIntLen(num);
    char* str_num = new char[len_int];

    uint8_t i = len_int - 1;
    while (num != 0) {
        str_num[i] = static_cast<char>('0' + num % 10);
        --i;
        num /= 10;
    }

    return str_num;
}

char* MakeBmpFilePath(char* directory, uint64_t iteration_number) {
    char* iteration_number_string = IntToStr(iteration_number);
    uint8_t len_num = GetIntLen(iteration_number);
    uint8_t len_directory = strlen(directory);

    char* bmp_file_path = new char[strlen(directory) + GetIntLen(iteration_number) + 11];
    strncpy(bmp_file_path, directory, strlen(directory));
    strncpy(bmp_file_path + len_directory, "/state_", 7);
    strncpy(bmp_file_path + len_directory + 7, iteration_number_string, len_num);
    strncpy(bmp_file_path + len_directory + 7 + len_num, ".bmp\0", 5);

    delete iteration_number_string;

    return bmp_file_path;
}


/* parsing and output to console */

void MakeAbelianSandpileModel(Arguments& args) {
    auto* abelianSandpileModel = new AbelianSandpileModel();

    std::ifstream tsv_file(args.input);

    if (!tsv_file.is_open()) {
        ErrorOutput(ErrorStatus::kNotAValidInputArg);
    }

    int16_t x;
    int16_t y;
    int64_t amount;

    int16_t min_x = kMaxOfInt16T;
    int16_t min_y = kMaxOfInt16T;

    int16_t max_x = kMinOfInt16T;
    int16_t max_y = kMinOfInt16T;

    while (tsv_file >> x >> y >> amount) {
        min_x = std::min(min_x, x);
        min_y = std::min(min_y, y);
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    };
    tsv_file.close();

    abelianSandpileModel->SetSize(max_x - min_x + 1, max_y - min_y + 1);

    tsv_file.open(args.input);
    while (tsv_file >> x >> y >> amount) {
        abelianSandpileModel->SetCell(x - min_x, y - min_y, amount);
    };
    tsv_file.close();

    std::ofstream bmp_file;
    for (uint64_t i = 0; i < args.max_iter; ++i) {
        abelianSandpileModel->Move();
        if (args.freq != 0 && i % args.freq == 0) {
            bmp_file.open(MakeBmpFilePath(args.output, i / args.freq + 1), std::ios::out | std::ios::binary);
            if (!bmp_file.is_open()) {
                ErrorOutput(ErrorStatus::kNotAValidOutputArg);
            }
            abelianSandpileModel->MakePhoto(bmp_file);
            bmp_file.close();
        }
    }

    if (args.freq == 0) {
        bmp_file.open(MakeBmpFilePath(args.output, 1), std::ios::out | std::ios::binary);
        if (!bmp_file.is_open()) {
            ErrorOutput(ErrorStatus::kNotAValidOutputArg);
        }
        abelianSandpileModel->MakePhoto(bmp_file);
        bmp_file.close();
    }

    delete abelianSandpileModel;
}


Arguments ParseArguments(int argc, char** argv) {
    /* parses arguments char* argv[] */
    Arguments args;

    if (argc == 1){ // if no arguments are entered
        ErrorOutput(ErrorStatus::kNoArgsEntered);
    }

    for (int16_t i = 1; i < argc; ++i) {

        if (IsItInputArg(argv[i])) {
            if (i + 1 >= argc) {
                ErrorOutput(ErrorStatus::kNotAValidInputArg);
            }

            strcpy(args.input, argv[i + 1]);
            ++i;

        } else if (IsItLongInputArg(argv[i])) {
            strcpy(args.input, argv[i] + kInputLongLen);

        } else if (IsItOutputArg(argv[i])) {
            if (i + 1 >= argc) {
                ErrorOutput(ErrorStatus::kNotAValidOutputArg);
            }

            strcpy(args.output, argv[i + 1]);
            ++i;

        } else if (IsItLongOutputArg(argv[i])) {
            strcpy(args.output, argv[i] + kOutputLongLen);

        } else if (IsItMaxIterArg(argv[i])) {
            if (i + 1 >= argc || !IsNumber(argv[i + 1])) {
                ErrorOutput(ErrorStatus::kNotAValidMaxIterArg);
            }

            char **end = nullptr;
            args.max_iter = strtoll(argv[i + 1], end, 10);
            ++i;

        } else if (IsItLongMaxIterArg(argv[i])) {
            if (!IsNumber(argv[i] + kMaxIterLongLen)) {
                ErrorOutput(ErrorStatus::kNotAValidMaxIterArg);
            }

            char **end = nullptr;
            args.max_iter = strtoll(argv[i] + kMaxIterLongLen, end, 10);

        } else if (IsItFreqArg(argv[i])) {
            if (i + 1 >= argc || !IsNumber(argv[i + 1])) {
                ErrorOutput(ErrorStatus::kNotAValidFreqArg);
            }

            char **end = nullptr;
            args.freq = strtoll(argv[i + 1], end, 10);
            ++i;

        } else if (IsItLongFreqArg(argv[i])) {
            if (!IsNumber(argv[i] + kFreqLongLen)) {
                ErrorOutput(ErrorStatus::kNotAValidFreqArg);
            }

            char **end = nullptr;
            args.max_iter = strtoll(argv[i] + kFreqLongLen, end, 10);

        } else {
            ErrorOutput(ErrorStatus::kUnknownArg);
        }

    }

    std::cout << args.input << " " << args.output << " " << args.max_iter << " " << args.freq << std::endl;

    if (strlen(args.input) == 0) {
        ErrorOutput(ErrorStatus::kNotAValidInputArg);
    }
    if (strlen(args.output) == 0) {
        ErrorOutput(ErrorStatus::kNotAValidOutputArg);
    }
    if (args.max_iter == 0) {
        ErrorOutput(ErrorStatus::kNotAValidMaxIterArg);
    }

    return args;
}

int main(int argc, char* argv[]) {
    Arguments args = ParseArguments(argc, argv);
    MakeAbelianSandpileModel(args);
    return EXIT_SUCCESS;
}