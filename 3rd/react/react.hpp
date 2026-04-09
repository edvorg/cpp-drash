// Minimal in-tree replacement for the original `cpp-react` library
// (https://github.com/edvorg/cpp-react), which is no longer hosted.
//
// drash only uses three things from react:
//   - react::Var<T>           : a value cell that notifies dependents on write
//   - react::RxRelaxed<T>     : a derived value cell, recomputed on demand
//   - react::link(ins...)
//        .reconnect(out, fn)  : wires `out` to be (re)computed from `ins...`
//                               via `fn` whenever any input changes
//
// This is a synchronous, single-threaded shim — sufficient for greng::Camera
// (the only consumer in this codebase). Lambdas passed to `reconnect` are
// invoked with the *unwrapped* values of their inputs (not the cells).

#pragma once
#ifndef DRASH_REACT_SHIM_HPP
#define DRASH_REACT_SHIM_HPP

#include <functional>
#include <tuple>
#include <utility>
#include <vector>

namespace react {

    namespace detail {

        class ObservableBase {
        public:
            ObservableBase() = default;
            ObservableBase(const ObservableBase&) = delete;
            ObservableBase& operator=(const ObservableBase&) = delete;
            ObservableBase(ObservableBase&&) = delete;
            ObservableBase& operator=(ObservableBase&&) = delete;
            virtual ~ObservableBase() = default;

            void AddDep(std::function<void()> dep) const {
                deps_.push_back(std::move(dep));
            }

            void NotifyDeps() const {
                // Iterate over a snapshot so a callback can safely
                // mutate this list (e.g. by writing to another cell).
                auto snapshot = deps_;
                for (auto& d : snapshot) {
                    d();
                }
            }

        private:
            mutable std::vector<std::function<void()>> deps_;
        };

    } // namespace detail

    template <class T>
    class Var : public detail::ObservableBase {
    public:
        using value_type = T;

        Var() = default;
        Var(const T& v) : value_(v) {}
        Var(T&& v) : value_(std::move(v)) {}

        Var& operator=(const T& v) {
            value_ = v;
            this->NotifyDeps();
            return *this;
        }

        Var& operator=(T&& v) {
            value_ = std::move(v);
            this->NotifyDeps();
            return *this;
        }

        operator const T&() const { return value_; }
        const T& operator()() const { return value_; }
        const T& getValue() const { return value_; }

    private:
        T value_{};
    };

    template <class T>
    class RxRelaxed : public detail::ObservableBase {
    public:
        using value_type = T;

        RxRelaxed() = default;

        void Set(const T& v) {
            value_ = v;
            this->NotifyDeps();
        }

        void Set(T&& v) {
            value_ = std::move(v);
            this->NotifyDeps();
        }

        operator const T&() const { return value_; }
        const T& operator()() const { return value_; }
        const T& getValue() const { return value_; }

    private:
        T value_{};
    };

    template <class... Inputs>
    class Link {
    public:
        explicit Link(Inputs&... ins) : inputs_(ins...) {}

        template <class Out, class Fn>
        void reconnect(RxRelaxed<Out>& target, Fn fn) {
            // Capture the input references and the user lambda by value, so the
            // resulting callback owns everything it needs.  The Link object
            // itself is a temporary returned from `link(...)`.
            auto inputs = inputs_;
            auto compute = [inputs, &target, fn]() {
                Out result = std::apply(
                    [&fn](auto&... cells) {
                        // Each cell is a Var/RxRelaxed; pass its current value.
                        return fn(cells()...);
                    },
                    inputs);
                target.Set(result);
            };

            // Compute once so the target has a meaningful initial value.
            compute();

            // Then re-run on any input change.
            std::apply(
                [&compute](auto&... cells) {
                    (cells.AddDep(compute), ...);
                },
                inputs_);
        }

    private:
        std::tuple<Inputs&...> inputs_;
    };

    template <class... Inputs>
    Link<Inputs...> link(Inputs&... ins) {
        return Link<Inputs...>(ins...);
    }

} // namespace react

#endif // DRASH_REACT_SHIM_HPP
