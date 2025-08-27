#!/bin/bash

#==============================================================================
# generate_docs.sh - TRLC Platform Library Documentation Generator
#==============================================================================
# 
# This script generates comprehensive API documentation for the TRLC Platform
# library using Doxygen. It creates HTML documentation with detailed API
# reference, examples, and cross-references.
#
# Requirements:
# - Doxygen (version 1.8.0 or later)
# - Optional: Graphviz for diagrams (dot tool)
#
# Usage:
#   ./generate_docs.sh [--clean] [--open]
#
# Options:
#   --clean  Remove existing documentation before generating
#   --open   Open generated documentation in default browser
#
# Author: TRLC Platform Team
# Version: 1.0.0
#==============================================================================

set -e  # Exit on any error

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
DOCS_OUTPUT_DIR="$PROJECT_ROOT/docs/generated"
DOXYGEN_CONFIG="$PROJECT_ROOT/Doxyfile"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Print usage information
print_usage() {
    cat << EOF
TRLC Platform Library Documentation Generator

Usage: $0 [OPTIONS]

Options:
    --clean     Remove existing documentation before generating
    --open      Open generated documentation in default browser after generation
    --help      Show this help message

Examples:
    $0                          # Generate documentation
    $0 --clean                  # Clean and generate documentation  
    $0 --clean --open           # Clean, generate, and open documentation

Requirements:
    - Doxygen (version 1.8.0 or later)
    - Optional: Graphviz for class diagrams

Output:
    HTML documentation will be generated in: $DOCS_OUTPUT_DIR/html/
EOF
}

# Check if required tools are available
check_dependencies() {
    log_info "Checking dependencies..."
    
    if ! command -v doxygen &> /dev/null; then
        log_error "Doxygen is not installed or not in PATH"
        log_error "Please install Doxygen (version 1.8.0 or later)"
        echo ""
        echo "Installation instructions:"
        echo "  Ubuntu/Debian: sudo apt-get install doxygen"
        echo "  CentOS/RHEL:   sudo yum install doxygen"
        echo "  macOS:         brew install doxygen"
        echo "  Windows:       Download from http://www.doxygen.nl/download.html"
        exit 1
    fi
    
    local doxygen_version=$(doxygen --version)
    log_success "Found Doxygen version: $doxygen_version"
    
    if command -v dot &> /dev/null; then
        local dot_version=$(dot -V 2>&1 | head -n1)
        log_success "Found Graphviz: $dot_version"
        log_info "Class diagrams and graphs will be generated"
    else
        log_warning "Graphviz (dot) not found - diagrams will be disabled"
        log_info "Install Graphviz for enhanced documentation with diagrams"
    fi
}

# Clean existing documentation
clean_docs() {
    if [ -d "$DOCS_OUTPUT_DIR" ]; then
        log_info "Cleaning existing documentation..."
        rm -rf "$DOCS_OUTPUT_DIR"
        log_success "Cleaned documentation directory: $DOCS_OUTPUT_DIR"
    else
        log_info "No existing documentation to clean"
    fi
}

# Validate project structure
validate_project() {
    log_info "Validating project structure..."
    
    if [ ! -f "$DOXYGEN_CONFIG" ]; then
        log_error "Doxygen configuration file not found: $DOXYGEN_CONFIG"
        exit 1
    fi
    
    if [ ! -d "$PROJECT_ROOT/include" ]; then
        log_error "Include directory not found: $PROJECT_ROOT/include"
        exit 1
    fi
    
    if [ ! -d "$PROJECT_ROOT/include/trlc/platform" ]; then
        log_error "Platform headers not found: $PROJECT_ROOT/include/trlc/platform"
        exit 1
    fi
    
    local header_count=$(find "$PROJECT_ROOT/include/trlc/platform" -name "*.hpp" | wc -l)
    if [ "$header_count" -eq 0 ]; then
        log_error "No header files found in: $PROJECT_ROOT/include/trlc/platform"
        exit 1
    fi
    
    log_success "Found $header_count header files"
    log_success "Project structure validation passed"
}

# Generate documentation
generate_documentation() {
    log_info "Generating documentation..."
    
    # Create output directory
    mkdir -p "$DOCS_OUTPUT_DIR"
    
    # Change to project root for relative paths
    cd "$PROJECT_ROOT"
    
    # Run Doxygen
    if doxygen "$DOXYGEN_CONFIG"; then
        log_success "Documentation generated successfully"
        
        # Check generated files
        local html_dir="$DOCS_OUTPUT_DIR/html"
        if [ -f "$html_dir/index.html" ]; then
            log_success "HTML documentation created at: $html_dir/index.html"
            
            # Count generated files
            local html_files=$(find "$html_dir" -name "*.html" | wc -l)
            local css_files=$(find "$html_dir" -name "*.css" | wc -l)
            local js_files=$(find "$html_dir" -name "*.js" | wc -l)
            
            log_info "Generated files:"
            log_info "  HTML files: $html_files"
            log_info "  CSS files:  $css_files"
            log_info "  JS files:   $js_files"
            
            # Calculate documentation size
            local doc_size=$(du -sh "$html_dir" 2>/dev/null | cut -f1 || echo "unknown")
            log_info "  Total size: $doc_size"
            
        else
            log_error "HTML documentation was not generated properly"
            exit 1
        fi
    else
        log_error "Documentation generation failed"
        exit 1
    fi
}

# Open documentation in browser
open_documentation() {
    local index_file="$DOCS_OUTPUT_DIR/html/index.html"
    
    if [ -f "$index_file" ]; then
        log_info "Opening documentation in default browser..."
        
        case "$(uname -s)" in
            Darwin)  # macOS
                open "$index_file"
                ;;
            Linux)
                if command -v xdg-open &> /dev/null; then
                    xdg-open "$index_file"
                elif command -v firefox &> /dev/null; then
                    firefox "$index_file" &
                elif command -v chromium-browser &> /dev/null; then
                    chromium-browser "$index_file" &
                else
                    log_warning "Could not determine how to open browser"
                    log_info "Please open manually: $index_file"
                fi
                ;;
            CYGWIN*|MINGW*|MSYS*)  # Windows
                start "$index_file"
                ;;
            *)
                log_warning "Unknown operating system"
                log_info "Please open manually: $index_file"
                ;;
        esac
        
        log_success "Documentation should open in your default browser"
    else
        log_error "Documentation index file not found: $index_file"
    fi
}

# Print documentation summary
print_summary() {
    log_success "Documentation generation completed!"
    echo ""
    echo "📖 Documentation Details:"
    echo "   Location: $DOCS_OUTPUT_DIR/html/"
    echo "   Main page: $DOCS_OUTPUT_DIR/html/index.html"
    echo ""
    echo "📋 What's included:"
    echo "   ✓ Complete API reference for all public functions and classes"
    echo "   ✓ Detailed parameter and return value documentation"
    echo "   ✓ Code examples and usage patterns"
    echo "   ✓ Cross-references between related functions"
    echo "   ✓ Platform and compiler compatibility information"
    echo "   ✓ Performance characteristics and thread safety notes"
    echo ""
    echo "🔗 Key sections:"
    echo "   - Main Page: Library overview and quick start guide"
    echo "   - Modules: Organized by functional area (architecture, compiler, etc.)"
    echo "   - Classes: Detailed class and struct documentation"
    echo "   - Files: Header file organization and dependencies"
    echo "   - Examples: Code examples and usage patterns"
    echo ""
    echo "💡 Tips:"
    echo "   - Use the search functionality to quickly find specific APIs"
    echo "   - Check the 'Related Pages' for comprehensive guides"
    echo "   - Browse by module for functional organization"
    echo "   - Look for code examples in function documentation"
}

#==============================================================================
# Main execution
#==============================================================================

main() {
    local clean_docs_flag=false
    local open_docs_flag=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --clean)
                clean_docs_flag=true
                shift
                ;;
            --open)
                open_docs_flag=true
                shift
                ;;
            --help)
                print_usage
                exit 0
                ;;
            *)
                log_error "Unknown option: $1"
                print_usage
                exit 1
                ;;
        esac
    done
    
    echo "🚀 TRLC Platform Library Documentation Generator"
    echo "================================================"
    echo ""
    
    # Execute generation steps
    check_dependencies
    validate_project
    
    if [ "$clean_docs_flag" = true ]; then
        clean_docs
    fi
    
    generate_documentation
    print_summary
    
    if [ "$open_docs_flag" = true ]; then
        echo ""
        open_documentation
    fi
    
    echo ""
    log_success "All done! 🎉"
}

# Run main function with all arguments
main "$@"
