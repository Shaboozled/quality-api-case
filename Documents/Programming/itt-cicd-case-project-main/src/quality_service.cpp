#include "quality_service.h"
#include <algorithm>
#include <cctype>
#include <numeric>
#include <iostream>

static bool isValidIntRange(int v, int min, int max) {
    return v >= min && v <= max;
}

static bool isValidString(const std::string& s, size_t maxLen = 50) {
    return !s.empty() && s.size() <= maxLen;
}

static std::string trim(const std::string& input) {
    size_t start = input.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = input.find_last_not_of(" \t\r\n");
    return input.substr(start, end - start + 1);
}

std::string QualityService::calculateGrade(int score) const {
    std::cout << "score is " << score << std::endl;
    if (!isValidIntRange(score, 0, 100)) return "INVALID INPUT";

    if (score < 0 || score > 100) return "Ugyldig";
    if (score >= 90) return "A";
    if (score >= 80) return "B";
    if (score >= 70) return "C";
    if (score > 60) return "D";
    return "F";
}

int QualityService::calculateDiscount(const DiscountRequest& request) const {
    if (!isValidIntRange(request.amount, 0, 100000)) return 0;
    if (!isValidIntRange(request.hourOfDay, 0, 23)) return 0;

    int discount = 0;
    if (request.amount > 100) discount = 10;
    if (request.amount >= 500) discount = 20;
    if (request.amount >= 1000) discount = 30;
    if (request.loyalCustomer) discount += 5;
    if (request.couponCode == "SAVE10") discount += 10;
    if (request.productionMode && request.hourOfDay >= 22) discount += 5;

    if (discount > 35) discount = 35;
    return discount;
}

bool QualityService::canBookSeats(const BookingRequest& request) const {
    if (!isValidIntRange(request.requestedSeats, 1, 1000)) return false;
    if (!isValidIntRange(request.currentReservations, 0, 10000)) return false;

    if (request.requestedSeats < 1) return false;
    if (request.requestedSeats <= 6) return true;
    if (request.hasSafetyOverride && request.currentReservations < 100) return true;
    return false;

}

std::string QualityService::formatUsername(const std::string& name) const {
    if (!isValidString(name, 100)) return "anonymous";
    std::string value = trim(name);
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c){ return std::tolower(c); });

    return value;
}

double QualityService::calculateSensorAverage(const std::vector<int>& values) const {
    if (values.empty()) return 0.0;
    int sum = std::accumulate(values.begin(), values.end(), 0);
    return static_cast<double>(sum) / values.size();
}

std::string QualityService::evaluateSensorHealth(const std::vector<int>& values) const {
    if (values.empty()) return "NO_DATA";

    for (int v : values) {
        if (v < 0 || v > 100) return "ERROR";
    }

    int minValue = *std::min_element(values.begin(), values.end());
    int maxValue = *std::max_element(values.begin(), values.end());

    double avg = calculateSensorAverage(values);

    if ((maxValue - minValue) > 40) return "UNSTABLE";
    if (avg < 20) return "WARNING";

    return "OK";
}
