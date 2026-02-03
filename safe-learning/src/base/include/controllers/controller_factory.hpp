#ifndef CONTROLLER_FACTORY_NODE_H
#define CONTROLLER_FACTORY_NODE_H

#include <memory>

#include "controllers/base.hpp"
#include "controllers/lqr.hpp"
#include "controllers/rl.hpp"


namespace safe_learning {
    // Adapted from: https://iifx.dev/en/articles/457774204/a-modern-c-factory-the-function-map-approach
    // Base factory class
    class ControllerFactory {
    public:
        virtual std::unique_ptr<Controller> create(const mjModel *m, mjData *d) = 0;
        virtual ~ControllerFactory() = default;
    };
    
    // Concrete factories
    class LqrFactory : public ControllerFactory {
    public:
        std::unique_ptr<Controller> create(const mjModel *m, mjData *d) override {
            std::unique_ptr<Controller> ctrl = LqrController::initialize(m, d);

            return ctrl;
        }
    };

    class RlFactory : public ControllerFactory {
    public:
        std::unique_ptr<Controller> create(const mjModel *m, mjData *d) override {
            std::unique_ptr<Controller> ctrl = RlController::initialize();

            return ctrl;
        }
    };

} // namespace safe_learning

#endif

