# Approach: Convex Hull Edge Matching with Hausdorf Distance 
Instead of comparing contours which can vary in details depending on the environment around the weapon, the intention is to compare convex hull edges instead since the intuition is that convex hull edges of a 3D modeled object viewing from effectively a fixed angle would persists through minor motions, fine details difference (i.e. different 'skins' of the same weapon), and lighting. 

## Findings
The contours seemse to have captured more key details. However, translating these details into convex hull edges lost a lot of identifying details. Additionally, the edge produced are inconsistent and I was unable to find an optimal parameter to get identifying contours that is also persistent across multiple test samples. I have also tested out edge matching on a weapon skin that is geometrically simpler, but the results remained similarly unsuccessful. 

## Conclusion
This approach is officially shelved going forward.
