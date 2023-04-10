#ifndef BEHAVIOR_TREE_NODE_H
#define BEHAVIOR_TREE_NODE_H

#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <cassert>
#include <numeric>

enum class NodeStatus
{
	Default,
	Success,
	Failure,
	Running
};

//BehaviorTreeNode class is the base class for all behavior tree nodes.
//It defines a basic interface for running a behavior tree nodeand getting its current status.
//The class is abstract, which means that it cannot be instantiated directlyand must be subclassed to create specific types of nodes.

class BehaviorTreeNode {
public:
    BehaviorTreeNode();
    virtual ~BehaviorTreeNode();
    virtual NodeStatus Run() = 0;

    NodeStatus TickNode();

    NodeStatus GetStatus() const { return currentStatus; }
    void SetNodeName(const std::string& name) { nodeName = name; }
    std::string GetNodeName() const { return nodeName; }

    static void SetDebugEnabled(bool value);

protected:
    virtual void ResetNode() {}

protected:
    NodeStatus currentStatus = NodeStatus::Default;
    static bool debug_enabled;
    std::string nodeName;
    
private:
    std::string statusToString(NodeStatus NodeStatus);
    
    
};


//Each node in the tree is connected to other nodes and evaluated sequentially during tree execution, 
//from the root to the leaves.The BehaviorTree class implements this structure and provides a method 
//to run the tree and change the root of the tree as needed.
class BehaviorTree : public BehaviorTreeNode
{
public:
    BehaviorTree();
    BehaviorTree(const std::shared_ptr<BehaviorTreeNode>& rootNode);

    NodeStatus Run() override;
    void SetRoot(const std::shared_ptr<BehaviorTreeNode> node);

private:
    std::shared_ptr<BehaviorTreeNode> root;
};

//The Condition Node represents a Boolean test that can evaluate to either true or false
//If the test evaluates to true, the node returns a SUCCESS status code, and if it evaluates 
//to false, it returns a FAILURE code.
class ConditionNode : public BehaviorTreeNode
{
public:
    ConditionNode();
    virtual ~ConditionNode();
    virtual NodeStatus Run() override;

    void SetTest(bool value) { test = value; }
    bool GetTest() const { return test; }

private:
    bool test = false;
};

//The SwitchConditionNode is a type of behavior tree node that has two child nodes, one for true and one for false conditions.
//SwitchConditionNode evaluates the given condition and executes the true child if the condition is true, or the false child otherwise.
class SwitchConditionNode : public BehaviorTreeNode
{
public:
    SwitchConditionNode(std::shared_ptr<BehaviorTreeNode> left, std::shared_ptr<BehaviorTreeNode> right);
    virtual ~SwitchConditionNode();
    NodeStatus Run() override;
    void SetCondition(bool condition) { m_condition = condition; }
    bool GetCondition() const { return m_condition; }

private:
    bool m_condition = false;
    std::shared_ptr<BehaviorTreeNode> leftChild; //trueChild
    std::shared_ptr<BehaviorTreeNode> rightChild; //falseChild
};

//A Selector Node examines its child components in sequence from left to right, 
//returning a SUCCESS code immediately when one of its children returns SUCCESS.
//If a child returns a FAILURE code, the selector continues examining its remaining children.
//If all of the selector's children return a FAILURE code, the selector returns a FAILURE code itself. 
class SelectorNode : public BehaviorTreeNode
{
public:
    SelectorNode();
    virtual ~SelectorNode();

    void AddChild(std::shared_ptr<BehaviorTreeNode> child);
    bool IsEmpty() const;
    NodeStatus Run() override;

private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
};


//The Sequence Node executes its child nodes sequentially, one after the other.
//If one of the nodes fails, the Sequence Node returns a failure stateand does not execute the remaining nodes.
//If a node succeeds, the Sequence Node continues to call the remaining nodes in sequence.
//If all nodes succeed, the Sequence Node returns a success state.
class SequenceNode : public BehaviorTreeNode
{
public:
    SequenceNode();
    virtual ~SequenceNode();

    void AddChild(std::shared_ptr<BehaviorTreeNode> child);
    bool IsEmpty() const;
    NodeStatus Run() override;

private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
}; 


//RandomUniformDistribution randomly selects one of its children to execute,
//with each child having an equal probability of being chosen.The selection
//is made using a uniform distribution.
class RandomUniformDistribution : public BehaviorTreeNode
{
public:
    RandomUniformDistribution(int numChildren);

    virtual ~RandomUniformDistribution();

    void AddChild(std::shared_ptr<BehaviorTreeNode> child);

    bool IsEmpty() const;

    NodeStatus Run() override;

private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    
    //generates uniformly distributed integers between 0 and the number of children.
    std::uniform_int_distribution<int> m_distribution;

    //std::default_random_engine that is used to generate random numbers
    std::default_random_engine m_eng;
};


//RandomWeightedDistribution selects a child node to execute based on a weighted random distribution.
//The user is required to provide a vector of weights that will be used to initialize the discrete distribution.
//The sum of the weights will be checked to ensure it equals 1.0.Then, when children are added to the distribution, 
//the weights will be added to the m_weights vector, but a new discrete distribution will not be created since the 
//initialization was done in the constructor.
class RandomWeightedDistribution : public BehaviorTreeNode
{
public:
    RandomWeightedDistribution();
    RandomWeightedDistribution(std::vector<float> weights);
    virtual ~RandomWeightedDistribution();
    void AddChild(std::shared_ptr<BehaviorTreeNode> child, float weight);
    bool IsEmpty() const;
    NodeStatus Run() override;

private:
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
    std::vector<float> m_weights;

    //The discrete distribution used to randomly select a child node based on its weight. 
    //The weights are normalized to ensure they sum up to 1.0.
    std::discrete_distribution<int> m_distribution;
    
    //std::default_random_engine that is used to generate random numbers
    std::default_random_engine m_eng;
};

//A decorator, as a wrapped component, affects exactly one componentand modifies its processing logic
class DecoratorNode : public BehaviorTreeNode
{
public:

    virtual ~DecoratorNode() {}

    void SetChild(std::shared_ptr<BehaviorTreeNode> node)
    { 
        child = node;
    }

    bool HasChild() const { return child != nullptr; }

protected:
    std::shared_ptr<BehaviorTreeNode> child;
};


//An Inverter decorator reverses the outcome of the behavior tree node it is attached to.
//If the wrapped node returns SUCCESS, the inverter will return FAILURE.
//If the wrapped node returns FAILURE, the inverter will return SUCCESS.
class Inverter : public DecoratorNode
{
public:

    NodeStatus Run() override
    {
        NodeStatus status = child->TickNode();

        if (status == NodeStatus::Success) {
            return NodeStatus::Failure;
        }
        else if (status == NodeStatus::Failure) {
            return NodeStatus::Success;
        }
 
        return status;
    }

};


//The Failer decorator always returns a failure status code, no matter what the child component's result is.
class Failer : public DecoratorNode
{
public:
    NodeStatus Run() override
    {
        child->TickNode();
        return NodeStatus::Failure;
    }
};


//The Succeeder decorator always returns a success status code, regardless of the child component's result.
class Succeeder : public DecoratorNode
{
public:
    NodeStatus Run() override
    {
        child->TickNode();
        return NodeStatus::Success;
    }
};


//The Loop decorator repeatedly executes its child node until the child node returns a SUCCESS state, 
//either infinitely or up to a specified limit.
class Repeater : public DecoratorNode
{
public:
    Repeater(int num_iterations);

    void ResetNode() override;
    NodeStatus Run() override;

protected:
    int num_iterations;
    int current_iteration = 0;
};


//RandomBernoulliDistribution represents a random Bernoulli distribution decorator node for use in behavior trees.
//It checks if it has a child node and randomly executes it based on a given probability.
class RandomBernoulliDistribution : public DecoratorNode {
public:
    RandomBernoulliDistribution();
    RandomBernoulliDistribution(float probability);

    NodeStatus Run() override;

    void SetProbability(float probability);
    float GetProbability() const;

private:
    float m_probability;
    //std::default_random_engine that is used to generate random numbers
    std::default_random_engine m_eng;
};

#endif