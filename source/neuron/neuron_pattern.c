
// Use Taylor expansion
double get_output(neuron_t *n, double *inputs) {
    double output = 0;
    for(uint32_t i=0; i<n->num_inputs) {
        double b = n->coeffs[i][0];
        double c = n->coeffs[i][1];
        output += pow(e, -((inputs[i]-b)**2)/(2*c)**2);
    }
    n->output = output/n->num_inputs;
}