---
layout: post
title:  "About jekyll"
date:   2017-3-10 12:00:00 +0800
categories: technical
cover_image: "Resources/default-cover-image.jpg"
---

This site is built by the help of jekyll and hosted on GitHub.

I would like to put some tips of jekyll here in case I forget them.

# 1. Jekyll/Liquid highlight tag bug.

Sometimes the \{\% highlight \%\} tag just doesn't work, and the following error is displayed:

Liquid Exception: highlight tag was never closed in ...

The solution to this problem is putting `excerpt_separator= ""` in the _config.yml file.

This solution is found at [http://obem.be/2013/09/13/jekyll-liquid-highlight-tag-bug.html](http://obem.be/2013/09/13/jekyll-liquid-highlight-tag-bug.html)


