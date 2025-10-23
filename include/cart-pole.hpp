#ifndef CART_POLE_NODE_H
#define CART_POLE_NODE_H

#include <math.h>
#include <float.h>

#include <Eigen/Dense>
using Eigen::VectorXd;
using Eigen::MatrixXd;


namespace safe_learning
{
    class CartPole
    {
        public:
            CartPole() 
            {
            }

        private:

        public:
            void setup_model();
            
    };
}

#endif
