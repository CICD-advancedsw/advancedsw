# Distributed Vending Machine System Test Cases

*Date: 04/07/2025*  
*OOPT STAGE 2000 / Distributed Vending Machine Team 1*

## Task Description
Analyze the provided distributed vending machine system test cases to extract all boundary values and generate comprehensive use cases that can be implemented with Google Test. Your analysis should be thorough and identify edge cases that might not be explicitly stated in the original test cases.

## Background Context
The system is a distributed vending machine network where machines communicate with each other
Test cases are organized into UI, Payment, Sales, Distribution, Pre-Payment, Communication, and Performance categories
We need to implement these tests using the Google Test framework

## Specific Requirements

### Boundary Value Analysis:

For each test case, identify all possible boundary values and edge conditions
Consider numeric limits (min/max quantities, prices, distances)
Consider time constraints (timeout conditions, response time limits)
Analyze string inputs (empty, max length, special characters)
Identify state boundaries (e.g., transitions between different machine states)


### Test Case Expansion:

For each original test case, generate 3-5 specific test scenarios that cover different boundary values
Ensure both positive and negative test cases are included
Consider sequence-dependent test cases (operations that must happen in a specific order)


### Google Test Implementation Guidelines:

Structure tests using TEST() or TEST_F() macros as appropriate
Suggest fixture classes where state needs to be maintained across tests
Include appropriate assertions (EXPECT_* vs ASSERT_*)
Recommend parameterized tests where applicable
Suggest mock objects for external dependencies (network, payment systems)


### Coverage Analysis:

Identify any coverage gaps in the original test cases
Suggest additional test cases to improve coverage
Ensure all system states and transitions are tested

### Expected Output Format
For each original test case, provide:

Boundary Value Analysis: List of identified boundary values and conditions
Expanded Test Cases: 3-5 specific test scenarios with:

Test name
Preconditions
Input values
Expected outcome
Google Test implementation snippet


Additional Use Cases: Any new use cases discovered during analysis that weren't in the original test cases

```
Example Output (For Illustration)
Original Test Case: TC-PAY-001 (Successful card payment)

Boundary Value Analysis:
- Payment amount: minimum allowed (1), maximum allowed (9999), exactly at card balance
- Quantity: minimum (1), maximum allowed per transaction (99)
- Card validity: just expired, just valid

Expanded Test Cases:

1. TC-PAY-001a: Minimum Payment Amount
   - Test name: TEST_F(PaymentTest, MinimumPaymentAmount)
   - Precondition: System initialized, cheapest drink selected
   - Input: quantity=1, card with sufficient balance
   - Expected: Payment success, drink dispensed
   - Implementation:
     ```cpp
     TEST_F(PaymentTest, MinimumPaymentAmount) {
       // Setup
       Beverage cheapestBeverage = inventory.getCheapestBeverage();
       Card validCard{CardInfo{"4111111111111111", "12/25", "123"}, 1000.0};
       
       // Exercise
       PaymentResult result = paymentProcessor.processPayment(cheapestBeverage, 1, validCard);
       
       // Verify
       EXPECT_TRUE(result.isSuccessful());
       EXPECT_EQ(result.getErrorMessage(), "");
       EXPECT_TRUE(dispenserController.wasDispensed(cheapestBeverage.getId(), 1));
     }
     ```

[Additional test cases would follow...]
```

Additional Use Cases:
- Network interruption during payment processing
- Card authorization timeout
- Partial stock fulfillment when quantity exceeds available stock
Special Considerations

Focus on system integration points where failures are more likely
Consider race conditions in the distributed system
Pay special attention to security aspects (authentication, authorization)
Consider performance degradation scenarios
Analyze potential data consistency issues between machines

Please provide a comprehensive analysis based on the test cases I've shared with you previously.

## UI Test Cases

| TC #       | Description                   | Input                           | Output                                                                            |
|------------|-------------------------------|--------------------------------|-----------------------------------------------------------------------------------|
| TC-UI-001  | Display all beverage menu     | Launch vending machine application | Output 20 types of beverages and 7 available beverages in the current machine      |
| TC-UI-002  | Display beverage stock info   | Enter beverage/quantity         | Output stock information for the selected beverage                                |
| TC-UI-003  | Select available beverage     | Input an available beverage     | Request payment + Output selected beverage details                                |
| TC-UI-004  | Select unavailable beverage   | Input an out-of-stock beverage  | Send broadcast message for stock check + Output guidance message                  |
| TC-UI-005  | Initial menu selection        | Input "Menu Purchase" number    | Output UI related to menu purchase                                                |
| TC-UI-006  | Initial menu selection        | Input "Pre-payment Pickup" number | Output UI related to pre-payment pickup                                          |

## Payment Test Cases

| TC #       | Description                           | Input                                                    | Output                                                 |
|------------|---------------------------------------|----------------------------------------------------------|--------------------------------------------------------|
| TC-PAY-001 | Successful card payment               | Input beverage/quantity + valid card information         | Payment success + Output beverage provision details     |
| TC-PAY-002 | Payment failure due to insufficient balance | Input beverage/quantity + card with insufficient balance | Output error message                                   |
| TC-PAY-003 | Payment failure due to invalid card   | Input beverage/quantity + invalid card information       | Output error message                                   |

## Direct Sales Test Cases

| TC #        | Description              | Input                                           | Output                                                                         |
|-------------|--------------------------|------------------------------------------------|--------------------------------------------------------------------------------|
| TC-SALES-001 | Direct sales processing | Input beverage/quantity + valid card information | Process payment + Output immediate beverage provision details + Update inventory |

## Stock Verification and Distribution Test Cases

| TC #       | Description                         | Input                                              | Output                                                                   |
|------------|-------------------------------------|---------------------------------------------------|--------------------------------------------------------------------------|
| TC-DIST-001 | Distributed VM stock verification   | Input out-of-stock beverage menu/quantity         | Send broadcast message + Wait for response                               |
| TC-DIST-002 | Stock verification response         | Response from other VMs (with stock and coordinates) | Process responses and identify VMs with stock + Calculate distance        |
| TC-DIST-003 | Calculate nearest VM                | Response data including coordinate information    | Identify nearest VM + Output user guidance message                        |
| TC-DIST-004 | Out-of-stock handling              | "No stock" responses from all VMs                 | Determine stock unavailability + Output user guidance message            |
| TC-DIST-005 | Equal distance VM handling          | Responses from VMs at identical distances         | Select VM based on lower ID priority + Output guidance message           |

## Pre-Payment Test Cases

| TC #       | Description                   | Input                                       | Output                                                               |
|------------|-------------------------------|---------------------------------------------|----------------------------------------------------------------------|
| TC-PRE-001 | Provide pre-payment option    | Input out-of-stock beverage/quantity        | Provide pre-payment option + Output guidance message                  |
| TC-PRE-002 | Process pre-payment           | Confirm pre-payment + Input valid card info | Process payment + Generate auth code + Output payment details         |
| TC-PRE-003 | Auth code generation          | -                                           | Generate 5-character random auth code (mixed case letters and numbers)|
| TC-PRE-004 | Guide to VM with stock        | Pre-payment completion response from VM     | Identify nearest VM + Output coordinate info/guidance message         |
| TC-PRE-005 | Pre-payment processing        | Input valid card information                | Send beverage sales processing request to target VM                   |
| TC-PRE-006 | Auth code verification        | User input of valid auth code               | Verify auth code and output beverage provision details                |
| TC-PRE-007 | Invalid auth code handling    | User input of invalid auth code             | Output error message + No beverage provision                          |

## Communication Test Cases

| TC #       | Description                  | Input                      | Output                                                     |
|------------|------------------------------|----------------------------|------------------------------------------------------------|
| TC-COM-001 | JSON message format validation | Trigger various messages | Send messages in specified JSON format and verify field values |
| TC-COM-002 | Broadcast message processing | Trigger broadcast message  | Message received and properly processed by all VMs         |

## Performance Test Cases

| TC #        | Description                  | Input                                   | Output                                                              |
|-------------|------------------------------|----------------------------------------|---------------------------------------------------------------------|
| TC-PERF-001 | Direct sales response time   | Select beverage + Input valid card info | Complete payment and beverage provision within 3 seconds            |
| TC-PERF-002 | Network communication response time | Trigger various messages         | Receive message response within 1 second + Efficiently process simultaneous messages |
