import os
import random
import re
import sys

DAMPING = 0.85
SAMPLES = 100000

def crawl(directory):
    """
    Parse a directory of HTML pages and check for links to other pages.
    Return a dictionary where each key is a page, and values are
    a list of all other pages in the corpus that are linked to by the page.
    """
    pages = dict()

    # Extract all links from HTML files
    for filename in os.listdir(directory):
        if not filename.endswith(".html"):
            continue
        with open(os.path.join(directory, filename)) as f:
            contents = f.read()
            links = re.findall(r"<a\s+(?:[^>]*?)href=\"([^\"]*)\"", contents)
            pages[filename] = set(links) - {filename}

    # Only include links to other pages in the corpus
    for filename in pages:
        pages[filename] = set(
            link for link in pages[filename]
            if link in pages
        )

    return pages


def transition_model(corpus, curr_page, damping_factor=DAMPING):
    """
    Return a probability distribution over which page to visit next,
    given a current page.

    With probability `damping_factor`, choose a link at random
    linked to by `page`. With probability `1 - damping_factor`, choose
    a link at random chosen from all pages in the corpus.
    """
    curr_links=[link for link in corpus[curr_page]]
    total_pages=len(corpus)
    num_links=len(curr_links)

    if num_links==0:
        probs={page:1/total_pages for page in corpus}

    else:
        undamped_prob=(1-damping_factor)/total_pages
        probs={page:undamped_prob for page in corpus}

        damped_prob=damping_factor/num_links 
        for link in curr_links:
            probs[link]+=damped_prob
    
    return probs
    # raise NotImplementedError

def sample_pagerank(corpus, damping_factor=DAMPING, n=SAMPLES):
    """
    Return PageRank values for each page by sampling `n` pages
    according to transition model, starting with a page at random.

    Return a dictionary where keys are page names, and values are
    their estimated PageRank value (a value between 0 and 1). All
    PageRank values should sum to 1.
    """
    # start from a random page and then using "transition model" get the probs 
    # and then using those "sample" another page. Repeat this for "n" samples and 
    # then check the number of times each page appears that would be its rank 

    pages=list(corpus.keys())
    curr_state=random.choices(pages)[0]
    num_times={page:0 for page in corpus}
    num_times[curr_state]+=1

    for _ in range(n-1):
        probs=transition_model(corpus,curr_state,damping_factor)
        next_state=random.choices(pages,list(probs.values()))[0]
        curr_state=next_state
        num_times[next_state]+=1
    
    pageranks={}
    for page,count in num_times.items():
        pageranks[page]=count/n

    return pageranks

    # raise NotImplementedError

def iterate_pagerank(corpus, damping_factor=DAMPING):
    """
    Return PageRank values for each page by iteratively updating
    PageRank values until convergence.

    Return a dictionary where keys are page names, and values are
    their estimated PageRank value (a value between 0 and 1). All
    PageRank values should sum to 1.
    """
    # Earlier sampling method was a markov chain based random surfer but in this 
    # we will use an iterative method for calculating page ranks.
    # We start by assuming rank of every page as 1/N and then recursively calculating 
    # the new pagerank values using previous ones using the formula. 
    # The idea is that this value will eventually converge upto some threshold.

    N=len(corpus)
    pageranks={page:1/N for page in corpus}

    undamped_prob=(1-damping_factor)/N
    changed=True
    while(changed):
        changed=False
        new_ranks=pageranks.copy()
        for page in corpus:
            parent_pages=[p for p in corpus if page in corpus[p]]
            rank_sum=0
            for parent in parent_pages:
                num_links=len(corpus[parent])
                if num_links==0:
                    num_links=N
                rank_sum+=pageranks[parent]/num_links

            new_rank=undamped_prob+damping_factor*rank_sum
            
            if abs(pageranks[page]-new_rank)>0.001:
                changed=True
            
            new_ranks[page]=new_rank
    
        pageranks=new_ranks

    return pageranks

    # raise NotImplementedError

def main():
    if len(sys.argv) != 2:
        sys.exit("Usage: python pagerank.py corpus")
    corpus = crawl(sys.argv[1])
    ranks = sample_pagerank(corpus, DAMPING, SAMPLES)
    print(f"PageRank Results from Sampling (n = {SAMPLES})")
    for page in sorted(ranks):
        print(f"  {page}: {ranks[page]:.4f}")
    ranks = iterate_pagerank(corpus, DAMPING)
    print(f"PageRank Results from Iteration")
    for page in sorted(ranks):
        print(f"  {page}: {ranks[page]:.4f}")


if __name__ == "__main__":
    main()
