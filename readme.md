# About  

**CDOCS** is a simple infrastructure for preprocessing Markdown, converting `.md` files into HTML, and providing a WebUI for collaborative documentation work.  

**Key Features**:  
1. **Lightweight** – Just one file. That's all you need to build HTML documentation.  
2. **Faster** than modern documentation systems based on JS or Python.  

## Implemented Features  

1. Variable parsing in `.md`.  
2. Inline `if` condition parsing in `.md`.  
3. Include block implementation in `.md`.  
4. If-block condition implementation in `.md`.  

## Planned Improvements  

- Optimize the Markdown parser. Reduce parsing time for 748 files to under 8 seconds.  
- Develop an MD-to-HTML converter.  

## Performance Test Results  

### Tag v1.0 MD Preprocessor  
- Files processed: 748  
- Time taken to preprocess variables: **2.705 s**  
- Time taken to preprocess inline if: **0.149 s**  
- Time taken to preprocess includes: **3.595 s**  
- Time taken to preprocess block_if: **5.427 s**  
- **Total execution time: 11.914 s**  

## Performance Optimization Plan  

1. Split variables into groups (maps).  
2. Implement a `swap` file.  
3. Implement a `threadpool`. 