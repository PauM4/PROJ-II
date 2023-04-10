#include "UriBehaviorTree.h"

//BehaviorTreeNode

std::string BehaviorTreeNode::statusToString(NodeStatus status)
{
    switch (status)
    {
    case NodeStatus::Default:
        return "Default";
        break;
    case NodeStatus::Success:
        return "Success";
        break;
    case NodeStatus::Failure:
        return "Failure";
        break;
    case NodeStatus::Running:
        return "Running";
        break;
    }
}

BehaviorTreeNode::BehaviorTreeNode()
{};

BehaviorTreeNode::~BehaviorTreeNode()
{};

bool BehaviorTreeNode::debug_enabled = false;

void BehaviorTreeNode::SetDebugEnabled(bool value)
{
    debug_enabled = value;
}

NodeStatus BehaviorTreeNode::TickNode()
{
	if (currentStatus != NodeStatus::Running) {
		ResetNode();
	}

	currentStatus = Run();

    if (!nodeName.empty() && debug_enabled)
    {
        std::cout << nodeName << ": " << statusToString(currentStatus) << std::endl;
    }
    
	return currentStatus;
}



//BehaviorTree
BehaviorTree::BehaviorTree() 
{
    nodeName = "BehaviorTree";
}

BehaviorTree::BehaviorTree(const std::shared_ptr<BehaviorTreeNode>& rootNode) : root(rootNode) {}

NodeStatus BehaviorTree::Run()
{
	return root->TickNode();
}

void BehaviorTree::SetRoot(const std::shared_ptr<BehaviorTreeNode> node)
{
	root = node;
}



//ConditionNode
ConditionNode::ConditionNode() 
{
    nodeName = "ConditionNode";
}
ConditionNode::~ConditionNode() {}

NodeStatus ConditionNode::Run()
{
    if (test)
    {
        currentStatus = NodeStatus::Success;
    }
    else
    {
        currentStatus = NodeStatus::Failure;
    }

    return currentStatus;
}



//SwitchConditionNode
SwitchConditionNode::SwitchConditionNode(std::shared_ptr<BehaviorTreeNode> left, std::shared_ptr<BehaviorTreeNode> right) :
    leftChild(left),
    rightChild(right)
{
    nodeName = "SwitchConditionNode";
}

SwitchConditionNode::~SwitchConditionNode()
{}

NodeStatus SwitchConditionNode::Run()
{
    if (m_condition)
    {
        auto status = leftChild->TickNode();
        if (debug_enabled)
        {
            std::cout << "Left Child Selected" << std::endl;
        }
        return status;
    }
    else
    {
        auto status = rightChild->TickNode();
        if (debug_enabled)
        {
            std::cout << "Right Child Selected" << std::endl;
        }
        return status;

    }
}


//SelectorNode
SelectorNode::SelectorNode()
{
    nodeName = "SelectorNode";
}
SelectorNode::~SelectorNode()
{}
void SelectorNode::AddChild(std::shared_ptr<BehaviorTreeNode> child)
{
    children.push_back(child);
}

bool SelectorNode::IsEmpty() const
{
    return children.empty();
}

NodeStatus SelectorNode::Run()
{
    for (auto& child : children) {
        NodeStatus status = child->TickNode();
        if (status == NodeStatus::Success) {
            return status;
        }
    }
    return NodeStatus::Failure;
}



//SequenceNoce
SequenceNode::SequenceNode()
{
    nodeName = "SequenceNode";
}
SequenceNode::~SequenceNode()
{}
void SequenceNode::AddChild(std::shared_ptr<BehaviorTreeNode> child)
{
    children.push_back(child);
}

bool SequenceNode::IsEmpty() const
{
    return children.empty();
}

NodeStatus SequenceNode::Run()
{
    for (auto& child : children) {

        NodeStatus status = child->TickNode();

        if (status == NodeStatus::Failure)
        {
            return status;
        }
    }

    return NodeStatus::Success;
}



//RandomUniformDistribution
RandomUniformDistribution::RandomUniformDistribution(int numChildren) : m_distribution(0, numChildren - 1)
{
    m_eng = std::default_random_engine(std::time(0));
    nodeName = "RandomUniformDistribution";

}

RandomUniformDistribution::~RandomUniformDistribution() {}

void RandomUniformDistribution::AddChild(std::shared_ptr<BehaviorTreeNode> child)
{
    children.push_back(child);
}

bool RandomUniformDistribution::IsEmpty() const
{
    return children.empty();
}

NodeStatus RandomUniformDistribution::Run()
{
    int index = m_distribution(m_eng);
    std::shared_ptr<BehaviorTreeNode> child = children[index];

    return child->TickNode();
}



//RandomWeightedDistribution

// Create a discrete distribution with default weights
RandomWeightedDistribution::RandomWeightedDistribution()
{
    // Initialize weights with 90% and 10% as default for the discrete_distribution to be initialized
    std::vector<float> weights; 
    weights.push_back(0.50);
    weights.push_back(0.50);
    std::discrete_distribution<int> defaultDist(weights.begin(), weights.end());
    m_distribution = defaultDist;
    
    // Initialize the random engine with the current time as seed
    m_eng = std::default_random_engine(std::time(0));

    // Check that the sum of the weights equals 1.0
    float sum = std::accumulate(weights.begin(), weights.end(), 0.0);
    assert(std::abs(sum - 1.0) < 1e-6 && "Weights do not sum up to 1.0");

    nodeName = "RandomWeightedDistribution";
}

 // Create a discrete distribution with the provided weights
RandomWeightedDistribution::RandomWeightedDistribution(std::vector<float> weights) :
    m_distribution(weights.begin(), weights.end())
{
    // Initialize the random engine with the current time as seed
    m_eng = std::default_random_engine(std::time(0));

    // Check that the sum of the weights equals 1.0
    float sum = std::accumulate(weights.begin(), weights.end(), 0.0);
    assert(std::abs(sum - 1.0) < 1e-6 && "Weights do not sum up to 1.0");

    nodeName = "RandomWeightedDistribution";
}

RandomWeightedDistribution::~RandomWeightedDistribution() {}

void RandomWeightedDistribution::AddChild(std::shared_ptr<BehaviorTreeNode> child, float weight)
{
    children.push_back(child);
    m_weights.push_back(weight);
    // Update the distribution with the new weights
    m_distribution = std::discrete_distribution<int>(m_weights.begin(), m_weights.end());
}

bool RandomWeightedDistribution::IsEmpty() const
{
    return children.empty();
}

NodeStatus RandomWeightedDistribution::Run()
{
    // Check that the sum of the weights equals 1.0
    float sum = std::accumulate(m_weights.begin(), m_weights.end(), 0.0);
    assert(std::abs(sum - 1.0) < 1e-6 && "Weights do not sum up to 1.0");


    // Select a random child node based on the weights
    int index = m_distribution(m_eng);
    std::shared_ptr<BehaviorTreeNode> child = children[index];

    // Execute the selected child node
    return child->TickNode();
}



//Repeater
Repeater::Repeater(int num_iterations) : num_iterations(num_iterations)
{
}

void Repeater::ResetNode()
{
    current_iteration = 0;
}

NodeStatus Repeater::Run()
{
    while (current_iteration < num_iterations) {
        child->TickNode();
        current_iteration++;
    }

    return NodeStatus::Success;
}



//RandomBernoulliDistribution
RandomBernoulliDistribution::RandomBernoulliDistribution() : m_probability(0.5)
{
    //// Initialize the random engine with the current time as seed
    m_eng = std::default_random_engine(std::time(0));
}

RandomBernoulliDistribution::RandomBernoulliDistribution(float probability) : m_probability(probability)
{
    assert(m_probability >= 0.0 && m_probability <= 1.0 && "Probability must be between 0 and 1");
    m_eng = std::default_random_engine(std::time(0));
}

NodeStatus RandomBernoulliDistribution::Run()
{
    assert(child && "RandomBernoulliDistribution: no child node");

    //Bernoulli distribution:a discrete probability distribution that describes the outcome of a random 
    //experiment that can only have two possible outcomes, usually labeled as success or failure. 
    //These two outcomes are mutually exclusive, meaning that only one of them can occur in a single trial.
    std::bernoulli_distribution bern_dist(m_probability);

    bool output = bern_dist(m_eng);

    if (output)
    {
        child->TickNode();
        return NodeStatus::Success;
    }
    else
    {
        return NodeStatus::Failure;
    }
}

void RandomBernoulliDistribution::SetProbability(float probability)
{
    m_probability = probability;
}

float RandomBernoulliDistribution::GetProbability() const
{
    return m_probability;
}