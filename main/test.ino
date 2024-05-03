/**
 * @file test.ino
 * @brief Test program for development
 * @details Run this code by uncomment the RUN_TEST_PROGRAM.
 */

void test(){
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  delay(1000);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
}