# Online-Exam-System-using-IPC
Write a C/C++ program that simulates a simple online exam system using message
passing for inter-process communication. The program should prompt the user for the
number of students taking the exam and the number of questions on the exam. The
program should then create a child process for each student and use message queues to
communicate the exam questions to the child process.
* Each child process should randomly answer the questions and send the answers
back to the parent process using another message queue.
* The parent process should collect all the answers and grade each student's exam.
* The program should display the grade for each student and the overall grade
distribution for the exam.
* After grading all the exams, the parent process should wait for all the child
processes to finish using the message queue synchronization mechanism. If any
child process fails to respond or terminates unexpectedly, the program should
display a warning message and take appropriate action to handle the situation
