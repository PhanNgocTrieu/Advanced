template <typename ...Args>
class Signal {
    public:
        Signal() = default;
        ~Signal() = default;
        void connect(Slot<Args...>& slot)
        {
            _connectedSlots.push_back(&slot);
        }

        void disconnect(Slot<Args...>* slot)
        {
            for (auto it = _connectedSlots.begin(); it != _connectedSlots.end(); ++it) {
                if (*it == slot) {
                    _connectedSlots.erase(it);
                    break;
                }
            }
        }

        void disconnect();

        void emit(Args... parameters)
        {
            for (auto &slot : _connectedSlots) {
                (slot)(parameters...);
            }
        }

    private:
        std::vector<Slot<Args...>*> _connectedSlots;
};


template <typename ...Args>
class Slot {
    public:
        Slot(const Slot&) = delete;
        Slot(Slot&&) = delete;
        Slot &operator(const Slot&) = delete;

        Slot(): _init(false) {};
        Slot(std::function<void(Args...)> func): _init(true), _callback(func) {};
        ~Slot() = default;

        // void connect(Signal<Args...>* signal);
        void operator()(Args... parameters)
        {
            this->call(parameters...);
        }
        void call(Args... parameters)
        {
            if (_init) {
                _callback(parameters...);
            }
        }
        void setSlotFunctions(std::function<void(Args...)> func)
        {
            _callback = func;
            _init = true;
        }

    private:
        bool _init;
        std::function<void(Args...)> _callback;
};


