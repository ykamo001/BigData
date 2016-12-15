#featureSearch
featureSearch is an artificial intelligence program that utilizes [k-fold cross validation](https://en.wikipedia.org/wiki/Cross-validation_(statistics)#k-fold_cross-validation) and [nearest neighbor algorithms](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm) to find the best features to use for classification.
 
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
