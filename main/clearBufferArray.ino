void clearBufferArray()                 // function to clear buffer array
{
    // clear all index of array with command NULL
  for (int i=0; i<count; i++) {
    buffer[i]='\0'; // use '\0' to clear the buffer
  }                 
}