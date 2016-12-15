#featureSearch
featureSearch is an artificial intelligence program that utilizes [k-fold cross validation](https://en.wikipedia.org/wiki/Cross-validation_(statistics)#k-fold_cross-validation) and [nearest neighbor algorithms](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm) to find the best features to use for classification.

##Installing and Running
To get featureSearch and run it, please open up a terminal and run these commands:
```
git clone https://github.com/ykamo001/featureSearch.git
cd featureSearch/
make
./src/featureSearch.out
```


Upon starting the program, it will ask you to enter the file that you would like to extrapolate data from; the data file should be in the following format:
```
2.0000000e+000  3.7080561e+000  3.0881362e+000  3.9684531e+000  3.6831232e+000  2.2850047e+000  2.9306701e+000  3.3161311e+000  3.1059884e+000  1.5443204e+000  3.9985351e+000
2.0000000e+000  2.8351111e+000  2.0471785e+000  3.5527371e+000  2.1615776e+000  2.0369645e+000  3.8633820e+000  2.0810766e+000  3.8470717e+000  3.8408205e+000  5.0040198e+000
1.0000000e+000  4.0593128e+000  3.2979390e+000  2.1946830e+000  3.1707453e+000  2.7453378e+000  3.1543968e+000  1.7549411e+000  4.2392857e+000  2.1683385e+000  3.2059945e+000
1.0000000e+000  3.7321650e+000  3.9232847e+000  3.6027575e+000  2.2511164e+000  2.1052860e+000  3.1606025e+000  2.3791584e+000  3.0531816e+000  2.7338837e+000  1.6365027e+000
2.0000000e+000  4.8891797e+000  4.2425334e+000  3.4677402e+000  2.3722136e+000  3.3300766e+000  4.7256750e+000  3.8125939e+000  3.6257853e+000  3.4577520e+000  2.9576445e+000

```
where the first column is the classification, which in this case is either 1 or 2, and the rest of the columns are the feature data.
I have kept some files in the [src](https://github.com/ykamo001/featureSearch/tree/master/src) directory for reference.


Once the data has been extracted, it will [normalize](https://en.wikipedia.org/wiki/Feature_scaling) the data, and then ask to choose between 3 search algorithms.

The 3 algorithms you can choose from are:

1. [Forward Search](https://en.wikipedia.org/wiki/Stepwise_regression#Main_approaches)
2. [Backward Search](https://en.wikipedia.org/wiki/Stepwise_regression#Main_approaches)
3. Custom Algorithm

The custom algorithm is the algorithm that I implemented, and is the forward search method, except that it uses 3 nearest neighbors to classify the set.

The program will finish, listing the features to combine that will classify with the highest accuracy.

Let's take a look at some of the sample data test runs:
```
Please enter the file you would like to extrapolate information from: src/small29.txt
Normalizing data, please wait..

Please select the search routine you would like to implement: 
1. Forward Search
2. Backward Search
3. Custom Search
option:1

Iteration: 1. Searching over features: 
adding feature: 1 has accuracy of: 66
adding feature: 2 has accuracy of: 72
adding feature: 3 has accuracy of: 61
adding feature: 4 has accuracy of: 73
adding feature: 5 has accuracy of: 65
adding feature: 6 has accuracy of: 84
adding feature: 7 has accuracy of: 68
adding feature: 8 has accuracy of: 59
adding feature: 9 has accuracy of: 61
adding feature: 10 has accuracy of: 63
Local max is: 84
Keeping feature: 6

Iteration: 2. Searching over features: 6 
adding feature: 1 has accuracy of: 77
adding feature: 2 has accuracy of: 91
adding feature: 3 has accuracy of: 77
adding feature: 4 has accuracy of: 88
adding feature: 5 has accuracy of: 81
adding feature: 7 has accuracy of: 88
adding feature: 8 has accuracy of: 81
adding feature: 9 has accuracy of: 82
adding feature: 10 has accuracy of: 78
Local max is: 91
Keeping feature: 2

Iteration: 3. Searching over features: 6 2 
adding feature: 1 has accuracy of: 78
adding feature: 3 has accuracy of: 88
adding feature: 4 has accuracy of: 87
adding feature: 5 has accuracy of: 84
adding feature: 7 has accuracy of: 83
adding feature: 8 has accuracy of: 78
adding feature: 9 has accuracy of: 81
adding feature: 10 has accuracy of: 85
Local max is: 88
Keeping feature: 3

Iteration: 4. Searching over features: 6 2 3 
adding feature: 1 has accuracy of: 81
adding feature: 4 has accuracy of: 78
adding feature: 5 has accuracy of: 82
adding feature: 7 has accuracy of: 76
adding feature: 8 has accuracy of: 72
adding feature: 9 has accuracy of: 78
adding feature: 10 has accuracy of: 83
Local max is: 83
Keeping feature: 10

Iteration: 5. Searching over features: 6 2 3 10 
adding feature: 1 has accuracy of: 74
adding feature: 4 has accuracy of: 76
adding feature: 5 has accuracy of: 77
adding feature: 7 has accuracy of: 78
adding feature: 8 has accuracy of: 63
adding feature: 9 has accuracy of: 76
Local max is: 78
Keeping feature: 7

Iteration: 6. Searching over features: 6 2 3 10 7 
adding feature: 1 has accuracy of: 71
adding feature: 4 has accuracy of: 79
adding feature: 5 has accuracy of: 77
adding feature: 8 has accuracy of: 67
adding feature: 9 has accuracy of: 77
Local max is: 79
Keeping feature: 4

Iteration: 7. Searching over features: 6 2 3 10 7 4 
adding feature: 1 has accuracy of: 68
adding feature: 5 has accuracy of: 77
adding feature: 8 has accuracy of: 61
adding feature: 9 has accuracy of: 68
Local max is: 77
Keeping feature: 5

Iteration: 8. Searching over features: 6 2 3 10 7 4 5 
adding feature: 1 has accuracy of: 64
adding feature: 8 has accuracy of: 58
adding feature: 9 has accuracy of: 70
Local max is: 70
Keeping feature: 9

Iteration: 9. Searching over features: 6 2 3 10 7 4 5 9 
adding feature: 1 has accuracy of: 62
adding feature: 8 has accuracy of: 55
Local max is: 62
Keeping feature: 1

Iteration: 10. Searching over features: 6 2 3 10 7 4 5 9 1 
adding feature: 8 has accuracy of: 56
Local max is: 56
Keeping feature: 8

Best features are: 6 2 
With accuracy of: 91
```

There are more test run reports in the [reports](https://github.com/ykamo001/featureSearch/tree/master/reports) directory.

###Findings
The datasets that pertained to me were for number 29. 
For the small file, the features that were useful were 6 and 2, with an accuracy of about 91%.
For the big file, the features that were useful were 20 and 26, with an accuracy of about 96.7%.
When running forward search, the best feature stand-alone was feature 6 with an accuracy of about 84%; in other words, when only using feature 6, compared to only using one of the other features, feature 6 was accurate about 84% of the time in classifying the elements using nearest neighbor.
Surprisingly, it was also the highest accuracy by a long shot compared to the other features, as the next best accuracy was about 73% for feature 4, and 72% with feature 2.
It's no wonder now that the best accuracy comes from choosing the features that already do well by themselves.
This is like a classic sports scenario, where combining some of the best players onto one team, A, will most likely make team A the best, as compared to having one great player on team B, while the rest of the team being average, making team B slightly better than they would be without that one great player, but no where near as good as team A.
After combining features 6 and 2, adding more features only brought down the accuracy, this is akin to the sports analogy, as the more features and players you add to one team, if the players are worse compared to the rest, then they will bring down the accuracy and performance of the overall team.


Let's take a look at backward search now.
When we use all the features, we have a disgustingly low accuracy, mostly due to the fact that we cannot distinguish the good from the bad.
But it's easy to notice that as we slowly remove one feature at a time, the accuracy starts to get better; therefore, intuitively, we can realize that if we remove the worst features one by one, we will get to a point where we will have a very good accuracy than from where we started off.
This is like cutting the worst player from the sports team, allowing only the good players to be on the team will ensure that the team is good.
There is one problem with using this technique however, is that we are only comparing it to the nearest neighbor of that particular point, and the nearest neighbor algorithm is very delicate to outliers.
This is like cutting a player on the team due to one bad performance on the week in which you need to cut players.
The player could indeed be bad, but not nearly as bad as someone else on the team.


If we classify a point wrong by only deciding on what point is nearest to that, then we will be ignoring the other closest neighbors that could classify the point correctly if the out-lier did not exist in the data point.
We can fix this problem, as I have done in my custom search algorithm, by simply keeping track of the three closest data points to that particular point, and then choosing to classify whatever the majority hold it to be.
This is akin to monitoring the players on multiple games, and choosing who the best player is based on multiple performances, or who the worst player is as well.
My algorithm is substantially better than the conventional nearest neighbor, because it accounts for outliers, and can give a better accuracy.
Although it might be difficult to see in my particular 29 dataset, it is actually abundantly clear when testing with dataset 80.
When running forward search with small dataset 80, we get an accuracy of about 91% with features 5 and 3, but when running my custom search algorithm, we get an accuracy of about 92% with features 5, 3, and 7.
This allows us to see that there was something misclassified in the normal nearest neighbor algorithm, and that there was also an out-lier when we combine features 5 and 3 together.
The down side to using my search function is that it could take a fraction of a second longer to run if it were on a big data set, where the time would increase by maybe 30 seconds, but would be worth the wait for a better quality answer.
I also found that normalizing the data is also very beneficial, as it produces accuracies to a finner level as compared to leaving the data in the form it was given.
This probably leads into the presence of some outliers, as one data point could be substantially greater in value as compared to the other values for that same particular feature.
In the end, there are a lot of conditions you need to look our for when trying to search, as there are a lot of options.
You could even use a different normalizing function, or choose a different size for the k-fold, or the amount of neighbors you ask, all of which contribute to the accruacy and quality of the answer and search you are looking for.
