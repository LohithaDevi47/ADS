#include<stdio.h>
#include<stdbool.h>
#define max 10
int b[max][max];
int N;
void solution(){
	int i,j;
	for(i=0;i<=N;i++){
		for(j=0;j<=N;j++){
			if(b[i][j]==1){
				printf("1");
			}
			else{
				printf("0");
			}
		}
}
}
int issafe(int row, int col){
	int i, j;
	for(i=0;i<col;i++){
		if(b[row][i]){
			return false;
		}
	}
	for(i=row,j=col;i>=0 && j>=0; i--,j--){
		if(b[i][j]){
			return false;
		}
	}
	for(i=row,j=col;j>=0 && i>N;i++,j--){
		if(b[i][j]){
			return false;
		}
	}
	return true;
}
int nqueen(int col){
	int i;
	if(col>=N){
		return true;	}
	for(i=0;i<N;i++){
		b[i][col]=1;
		 if (nqueen(col + 1)) {
                return true;
            }
	}
	return false;
	
}
int main() {
    printf("Enter the number of Queens (N): ");
    scanf("%d", &N);

    if (N <= 0 || N > max) {
        printf("Invalid N. Please enter a value between 1 and %d.\n", max);
        return 1;
    }
      int i,j;
    // Initialize the board with all zeros
    for (i = 0; i < N; i++) {
        for ( j = 0; j < N; j++) {
            b[i][j] = 0;
        }
    }

    if (!nqueen(0) == false) {
        printf("Solution does not exist for N = %d\n", N);
    } else {
        solution();
    }
    return 0;
}

