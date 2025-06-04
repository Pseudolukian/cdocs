# About  

**CDOCS** is a simple infrastructure for preprocessing Markdown, converting `.md` files into HTML, and providing a WebUI for collaborative documentation work.  

**Key Features**:  
1. **Lightweight** – Just one file. That's all you need to build HTML documentation.  
2. **Faster** than modern documentation systems based on JS or Python.  

## Implemented Features  

### Variables

Using `{{ var_name }}` template for add vars in the `.md`. Vars split by the group: default group -- main, other groups ...

### Inline if conditions

### Block if conditions

### Block include

## Planned Improvements  
  
- Develop an MD-to-HTML converter.  

## Performance Test Results  

### Tag v1.0 MD Preprocessor  
- Files processed: 748  
- Time taken to preprocess variables: 2.705 s  
- Time taken to preprocess inline if: 0.149 s  
- Time taken to preprocess includes: 3.595 s  
- Time taken to preprocess block_if: 5.427 s  
- Total execution time: **11.914 s**  

### Tag v1.5 Optimized
- Files processed: 748
- Time taken to compile regex: 0.003 s
- Time taken to load conf and vars: 0.002 s
- Time taken to get file list to preproc: 0.044 s
- Time taken to preprocess vars: 1.659 s
- Time taken to preprocess inline if: 0.224 s
- Time taken to preprocess include: 1.452 s
- Time taken to preprocess block_if: 0.601 s
- Total execution time: **3.99 s**

### Tag v1.7 Boost more performance
- Files processed: 748
- Time taken to compile regex: 0.006 s
- Time taken to load conf and vars: 0.007 s
- Time taken to get file list to preproc: 0.058 s
- Time taken to preprocess vars: 0.073 s
- Time taken to preprocess inline if: 0.05 s
- Time taken to preprocess include: 0.233 s
- Time taken to preprocess block_if: 0.558 s
- Total execution time: **0.989 s**

## Performance Optimization Plan  

1. Split variables into groups (maps).  
2. Implement a `swap` file.  
3. Implement a `threadpool`. 