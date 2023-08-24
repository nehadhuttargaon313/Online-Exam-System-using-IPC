#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>

using namespace std;

#define MAX_QUESTIONS 10
#define MAX_ANSWERS 10

struct Question {
    long type;
    char text[50];
};

struct Answer {
    long type;
    char text[50];
};

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    int msgqid_q[n];
    int msgqid_a[n];
    //msgqid[0] = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    pid_t pid[n];
    for (int i = 0; i < n; i++) {
        msgqid_q[i] = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
        if (msgqid_q[i] < 0) {
            cerr << "msgget failed" << endl;
            return 1;
        }
        msgqid_a[i] = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
        if (msgqid_a[i] < 0) {
            cerr << "msgget failed" << endl;
            return 1;
        }
    }
    cout << "created\n";
    char opt[4][50] = {"A", "B", "C", "D"};
    for (int i = 0; i < n; i++) {
        //msgqid[i] = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
        pid[i] = fork();

        if (pid[i] < 0) {
            cerr << "Fork failed" << endl;
            return 1;
        } else if (pid[i] == 0) {
            // Child process
            srand(time(NULL) + i+1);
            Question question;
            Answer answer;
            for (int j = 0; j < m; j++) {
                // Receive question from parent
                msgrcv(msgqid_q[i], &question, sizeof(Question) - sizeof(long), j+1, 0);
                cout << "Child " << i + 1 << " : qn rcvd - " << question.text << endl;
                // Answer question
                int ans = rand()%4;
                strcpy(answer.text, opt[ans]);
                cout << "Child " << i + 1 << " : Ans chosen for qn - " << j+1 <<  " " << answer.text << endl;
                //strcat(answer.text, question.text);
                answer.type = question.type;

                // Send answer to parent
                msgsnd(msgqid_a[i], &answer, sizeof(Answer) - sizeof(long), 0);
            }

            msgrcv(msgqid_q[i], &question, sizeof(Question) - sizeof(long), m, 0);
            cout << "Child " << i + 1 <<  " " << question.text << endl; 
            exit(0);
        }
    }

    // Parent process
    Question question;
    Answer answer;
    vector<int> scores(n, 0);
    vector<int> ans(m, rand()%4);
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            // Send question to child i
            strcpy(question.text, "Question ");
            strcat(question.text, to_string(j+1).c_str());
            question.type = j+1;
            msgsnd(msgqid_q[i], &question, sizeof(Question) - sizeof(long), 0);

            // Receive answer from child i
            msgrcv(msgqid_a[i], &answer, sizeof(Answer) - sizeof(long), j+1, 0);
            if(strcmp(answer.text, opt[ans[j]])==0)
                scores[i] += 1;
            
        }
    }

    for (int i = 0; i < n; i++) {
        strcpy(question.text, "scored ");
        strcat(question.text, to_string(scores[i]).c_str());
        question.type = m;
        msgsnd(msgqid_q[i], &question, sizeof(Question) - sizeof(long), 0);
    }

    // Wait for child processes to finish
    for (int i = 0; i < n; i++) {
        waitpid(pid[i], NULL, 0);
        int ret = msgctl(msgqid_q[i], IPC_RMID, NULL);
        //cout << "\n\nret value is " << ret << endl << endl;
        if(ret==-1){
            cout << "remove error\n";
        }
        ret = msgctl(msgqid_a[i], IPC_RMID, NULL);
        if(ret==-1){
            cout << "remove error\n";
        }
    }

    // for(int i = 0; i < n; i++){
    //     cout << i+1 << " - " << scores[i] << endl;
    // }

    int binSize = (m + 1)/10;
    vector<int> dist(10, 0);

    for(int i = 0; i < n; i++){
        if(scores[i] >= 0 && scores[i] < binSize)
            dist[0]++;
        else if(scores[i] >= binSize && scores[i] < 2*binSize)
            dist[1]++;
        else if(scores[i] >= 2*binSize && scores[i] < 3*binSize)
            dist[2]++;
        else if(scores[i] >= 3*binSize && scores[i] < 4*binSize)
            dist[3]++;
        else if(scores[i] >= 4*binSize && scores[i] < 5*binSize)
            dist[4]++;
        else if(scores[i] >= 5*binSize && scores[i] < 6*binSize)
            dist[5]++;
        else if(scores[i] >= 6*binSize && scores[i] < 7*binSize)
            dist[6]++;
        else if(scores[i] >= 7*binSize && scores[i] < 8*binSize)
            dist[7]++;
        else if(scores[i] >= 8*binSize && scores[i] < 9*binSize)
            dist[8]++;
        else
            dist[9]++;
    }

    for(int i = 0; i < 10; i++){
        //cout << "[ " << i*binSize << ", " << (i+1) * binSize-1 << " ] ---- " << dist[i] << endl; 
        printf("[ %5d , %5d ]     ---- %5d\n", i*binSize, (i+1)*binSize, dist[i]);
    }

    int maxPossibleScore = m;


    return 0;
}
