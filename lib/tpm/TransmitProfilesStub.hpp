namespace ARIASDK_NS_BEGIN {

    void TransmitProfiles::UpdateProfiles(const std::vector<TransmitProfileRules>&) {};

    void TransmitProfiles::EnsureDefaultProfiles() {};
		
    bool TransmitProfiles::load(const std::string&) { return false; };

    bool TransmitProfiles::load(const std::vector<TransmitProfileRules>&) { return false; };

    void TransmitProfiles::reset() { };

    bool TransmitProfiles::setDefaultProfile(const TransmitProfile) { return false; };

    bool TransmitProfiles::setProfile(const std::string&) { return false; }

    std::string& TransmitProfiles::getProfile() { static std::string def = ""; return def; }

    bool TransmitProfiles::updateStates(NetworkCost, PowerSource) { return true; };

    void TransmitProfiles::getTimers(std::vector<int>& out)
    {
        out = { 1, 2, 4 };
    }

    bool TransmitProfiles::isTimerUpdateRequired() { return false; }

    TransmitProfiles::~TransmitProfiles() {};

} ARIASDK_NS_END
